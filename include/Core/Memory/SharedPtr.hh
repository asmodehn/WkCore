#ifndef SHAREDPTR_HH
#define SHAREDPTR_HH

//this class can only be read by C++ compiler.
#ifdef __cplusplus

#include <cassert>
#include <iostream>
#include <algorithm>

namespace Core
{
	/** @namespace memory
	@brief NULL
	*/
	namespace memory
	{

		/** @brief Function object class, whose function-like invokation takes an object of type T* and deletes it.
		 *		This class is specifically designed to be used as the type for the stored deleter object in SharedPtr instantiations or to declare objects passed to SharedPtr's constructors.
		 *		The non-specialized version simply uses delete for the deletion operation.
		 */
		template <class T>
		class default_delete
		{
		public:
		  void operator ()( T const * p) throw()
		  { 
			::delete p, p=0; 
		  }
		};

		/** @brief Function object class, whose function-like invokation takes an object of type T* and deletes it.
		 *		This class is specifically designed to be used as the type for the stored deleter object in SharedPtr instantiations or to declare objects passed to SharedPtr's constructors.
		 *		The specialization for arrays with runtime length uses delete[].
		 */
		template <class T>
		class default_delete<T []>
		{
		public:
		  void operator ()( T const * p) throw()
		  { 
			::delete[] p, p=0; 
		  }
		};

		//interface for type erasure of control block
		typedef struct ctrl_block {
			virtual ~ctrl_block() {}
			virtual void shared_del() = 0;
			virtual void shared_cpy() = 0;
			virtual long get_refCount() const = 0;
		}ctrl_block;

	/***************************************************************************************
	 * @class SharedPtr
	 * @brief SharedPtr class represent a reference counting shared pointer. 
	 *  Only when the reference count goes down to 0 ( all smart pointer have been deleted )
	 *  the actual pointer is being actually deleted.
	 *
	 *  The design is supposed to follow std::shared_ptr, for future simplification using directly C++11 standard library.
	 *  except that we do not protect against reference cycle (ie no weak_ptr here).
	 *
	 *  A simple guideline that nearly eliminates the possibility of memory leaks is: always use a named smart pointer variable to hold the result of new.
	 *  Every occurence of the new keyword in the code should have the form:
	 * 		SharedPtr<T> p(new Y);
	 * 	It is, of course, acceptable to have T and Y be the same type, or passing arguments to Y's constructor is also OK.
	 *  If you observe this guideline, it naturally follows that you will have no explicit deletes; try/catch constructs will be rare.
	 *
	 * @date 2013/11/01
	 * @author AlexV
	 */
	template< class T > //specifying Deleter tempate argumet to avoid implementing type erasure...
	class SharedPtr
	{
	public:

		/** @brief default constructor creates and empty smart pointer
		 *  It does not throw any exception.
		 */
		SharedPtr() throw()
			: m_ptr(0),m_ctrl(0)
		{}

		/** @brief explicit constructor from raw pointer
		 *  It does not throw any exception.
		 *	@param ptr pointer. Y must be a complete type and ptr must be convertible to T*.
		 *  @warning : arrays ( which should use delete[] ) are not supported by this constructor. you need to also pass a delete command object.
		 *  @warning : There is no way to distinguish multiple shared pointer using the same pointer -> BE CAREFUL.
		 */
		template< class Y >
		explicit SharedPtr(Y* ptr) throw()
			: m_ptr(ptr), m_ctrl( (ptr)?new ctrl_block_delimpl<>(ptr):0 ) //making sure ptr is a valid one
		{}

		/** @brief explicit constructor from raw pointer
		 *  It does not throw any exception.
		 *	@param ptr pointer
		 *	@param deleter a deleter function to call when the object should be deleted.
		 *    by default we use a simple delete call.
		 *  @warning : There is no way to distinguish multiple shared pointer using the same pointer -> BE CAREFUL.
		 */
		template <class Y, class Deleter>
		explicit SharedPtr(Y* ptr, Deleter delf ) throw()
			: m_ptr(ptr),m_ctrl( (ptr)?new ctrl_block_delimpl<Deleter>(ptr,delf):0 ) //making sure ptr is a valid one
		{}

		/** @brief Copy contructor
			@param c_rsp pointer to a SharedPtr
		*/
		SharedPtr(const SharedPtr& c_rsp) throw()
			: m_ptr(c_rsp.m_ptr)
			, m_ctrl(c_rsp.m_ctrl)//sharing control block
		{
			if ( m_ctrl ) m_ctrl->shared_cpy();
		}

		/** @brief Copy contructor
			This templated overload participate in the overload resolution if and only if Y* is implicitly convertible to T*
			@param c_rsp pointer to a SharedPtr
		*/
		template<class Y>
		SharedPtr(const SharedPtr<Y>& c_rsp) throw()
			: m_ptr(c_rsp.m_ptr)
			, m_ctrl(c_rsp.m_ctrl)//sharing control block
		{
			if ( m_ctrl ) m_ctrl->shared_cpy();
		}

		/** @brief Aliasing contructor
		 * The object does not own p, and will not manage its storage. Instead, it co-owns c_rsp's managed object and counts as one additional use of c_rsp.
		 * It will also delete c_rsp's pointer on release (and not p).
		 * It can be used to point to members of objects that are already managed. It is used for SharedPtr_casting for exemple.
		 *	@param c_rsp pointer to a SharedPtr
		 */
		SharedPtr(const SharedPtr& c_rsp, T* p) throw()
			: m_ptr(p)
			, m_ctrl(c_rsp.m_ctrl)//sharing control block
		{
			if ( m_ctrl ) m_ctrl->shared_cpy();
		}

		/** @brief Aliasing contructor
		 * The object does not own p, and will not manage its storage. Instead, it co-owns c_rsp's managed object and counts as one additional use of c_rsp.
		 * It will also delete c_rsp's pointer on release (and not p).
		 * It can be used to point to members of objects that are already managed. It is used for SharedPtr_casting for exemple.
		 * This templated overload participate in the overload resolution if and only if Y* is implicitly convertible to T*
		 *	@param c_rsp pointer to a SharedPtr
		 */
		template<class Y>
		SharedPtr(const SharedPtr<Y>& c_rsp, T* p) throw()
			: m_ptr(p)
			, m_ctrl(c_rsp.m_ctrl)//sharing control block
		{
			if ( m_ctrl ) m_ctrl->shared_cpy();
		}
		

		/** @brief destructor
		*/
		~SharedPtr() throw()
		{
			if ( m_ctrl ) m_ctrl->shared_del();
		}

		/** @brief Shares ownership of the object managed by r. If r manages no object, *this manages no object too.
			 Equivalent to shared_ptr p(r).swap(*this).
			@param r another smart pointer to share the ownership to or acquire the ownership from
			@return *this
			@todo this implementation can be optimized to meet the requirements without creating a temporary object.
		*/
		SharedPtr& operator=(const SharedPtr& r)
		{
			SharedPtr(r).swap(*this);
			return *this;
		}

		/** @brief Shares ownership of the object managed by r. If r manages no object, *this manages no object too.
			 Equivalent to shared_ptr p(r).swap(*this).
			 This templated overload participate in the overload resolution if and only if Y* is implicitly convertible to T*
			@param r another smart pointer to share the ownership to or acquire the ownership from
			@return *this
			@todo this implementation can be optimized to meet the requirements without creating a temporary object.
		*/
		template<class Y>
		SharedPtr<Y>& operator=(const SharedPtr<Y>& r)
		{
			SharedPtr<Y>(r).swap(*this);
			return *this;
		}

		/**
		 * @fn void reset() throw()
		 * @brief 
		 * Replaces the managed object with an object pointed to by ptr. 
		 * If *this already owns an object and it is the last shared_ptr owning it, the object is destroyed through the owned deleter.
		 * Releases the ownership of the managed object, if any. After the call, *this manages no object. Equivalent to shared_ptr().swap(*this);
		 *
		 */
		void reset() throw()
		{
			SharedPtr<T>().swap(*this);
			//release the temporary smart pointer. deleting the object if needed.
		}

		/**
		 * @fn void reset(T* newptr)
		 * @brief 
		 * Replaces the managed object with an object pointed to by ptr. The delete expression is used as deleter.
		 * Proper delete expression corresponding to the supplied type is always selected, this is the reason why the function is implemented as template using a separate parameter Y.
		 * Y must be a complete type and implicitly convertible to T. deleter must be callable for T.
		 * If *this already owns an object and it is the last shared_ptr owning it, the object is destroyed through the owned deleter.
		 * Releases the ownership of the managed object, if any. After the call, *this manages no object. Equivalent to shared_ptr().swap(*this);
		 *
		 */
		template< class Y >
		void reset(Y* newptr)
		{
			//swap the contents
			SharedPtr<T>(newptr).swap(*this);
			//release the temporary smart pointer. deleting the object if needed ( with the proper deleter ).
		}

		/**
		 * @fn void reset(Y* newptr,SharedPtr<Y>::deleter delf )
		 * @brief 
		 * Replaces the managed object with an object pointed to by ptr. By default, delete expression is used as deleter.
		 * Proper delete expression corresponding to the supplied type is always selected, this is the reason why the function is implemented as template using a separate parameter Y.
		 * Y must be a complete type and implicitly convertible to T. deleter must be callable for T.
		 * If *this already owns an object and it is the last shared_ptr owning it, the object is destroyed through the owned deleter.
		 * Releases the ownership of the managed object, if any. After the call, *this manages no object. Equivalent to shared_ptr().swap(*this);
		 *
		 */
		template< class Y, class Deleter >
		void reset(Y* newptr, Deleter delf )
		{
			//swap the contents
			SharedPtr<T>(newptr,delf).swap(*this);
			//release the temporary smart pointer. deleting the object if needed ( with the proper deleter ).
		}

		/**
		 * @fn void swap(SharedPtr& newptr) throw()
		 * @brief Exchanges the contents of *this and r
		 * @param r - smart pointer to exchange the contents with
		 * @warning : this is not thread safe !
		 */
		void swap(SharedPtr& newptr) throw()
		{
			std::swap(m_ctrl,newptr.m_ctrl);
			std::swap(m_ptr,newptr.m_ptr);
		}

		/**
		 * @fn T* get() const
		 * @brief Returns a pointer to the managed object.
		 * @return a pointer to the managed object.
		 */
		T* get() const
		{
			return  m_ptr;
		}

		///>DEPRECATED
		T* GetPtr() const
		{
			return  get();
		}

		/**
		 * @brief Checks if *this is the only shared_ptr instance managing the current object, i.e. whether use_count() == 1.
		 * Empty pointers are never unique (as they do not own any pointers).
		 * Unique shared_ptr objects are responsible to delete their managed object if they release this ownership (see the destructor).
		 * @return true if *this is the only shared_ptr instance managing the current object, false otherwise.
		 */
		bool unique() const throw() // never throws
		{
			return m_ctrl && m_ctrl->get_refCount() == 1;
		}

		/**
		 * @brief Returns the number of different shared_ptr instances (this included) managing the current object.
		 * If there is no managed object, ?0? is returned. Reference count is a 'long' to match STL API
		 * @return the number of shared_ptr instances managing the current object or ?0? if there is no managed object.
		 */
		long use_count() const throw() // never throws
		{
			return (long) (m_ctrl)?m_ctrl->get_refCount(): 0 ;
		}

		/**
		 * @brief Checks if *this manages an object, i.e. whether get() != 0.
		 * @return true if *this manages an object, false otherwise.
		 */
		//explicit operator bool() const throw() // never throws
		//{
		//	return (0 != get());
		//}
		//C++11 not supported by VS2010
		// similar usage :
		/**
		 * @brief Checks if *this manages no object, i.e. whether get() == 0.
		 * @return true if *this manages no object, false otherwise.
		 *
		 */
		bool IsNull() const throw()
		{
			return (0 == get());
		}

		/** @brief Dereferences pointer to the managed object.
			@return reference to the managed object.
		*/
		T& operator*() const
		{
			assert(NULL != m_ptr);
			return *m_ptr;
		}

		/** @brief Dereferences pointer to the managed object.
			@return pointer to the managed object.
		*/
		T* operator->() const
		{
			assert(NULL != m_ptr); 
			return  m_ptr;
		}

	private:
		T* m_ptr; ///< pointer as a member variable

		//private class to tightly control how an instance is created / shared / suicided
		template <class Deleter = default_delete<T> >
		class ctrl_block_delimpl : public ctrl_block
		{
		public:
			ctrl_block_delimpl() : m_ptr(0),m_delCallback(Deleter()),m_refCount(0) {}
			explicit ctrl_block_delimpl(T* ptr, Deleter dltr = default_delete<T>() ) : m_ptr(ptr),m_delCallback(dltr),m_refCount(1) {}

			virtual long get_refCount() const
			{
				return m_refCount;
			}
			virtual void shared_cpy()
			{
				++m_refCount;
			}
			virtual void shared_del()
			{
				if(--m_refCount==0 /* && m_refWeakCount == 0 */)
				{
					delete this;
				}
			}
			//void weak_del() { if(m_refCount==0 /* && --m_refWeakCount == 0 */) delete this; }
			virtual ~ctrl_block_delimpl()
			{ 
				m_delCallback(m_ptr),m_ptr=0;
			}
		public:
			T* m_ptr;
			Deleter m_delCallback; ///< delete function pointer to call when reference count reach 0.
			long m_refCount; ///< reference count as long to match STL API
			//long m_refWeakCount;
		private: // forbidding copy constructor ( it should not be duplicated, it should be shared )
			ctrl_block_delimpl(const ctrl_block_delimpl& cbl ) : m_ptr(cbl.m_ptr),m_delCallback(cbl.m_delCallback),m_refCount(cbl.m_refCount) {}
		};

		ctrl_block* m_ctrl;

		// Any other type of MyClass is a friend.
		template <typename U>
		friend class SharedPtr;

		template <class D, class U>
		friend D* get_deleter (const SharedPtr<U>& sp) throw();
		
	};

	template <class charT, class traits, class Y>
	std::basic_ostream<charT,traits>& operator<< ( std::basic_ostream<charT,traits>& os, const SharedPtr<Y>& x )
	{
		return os << x.get();
	}

	/**
	 * @brief Specializes the std::swap algorithm for std::shared_ptr. Swaps the pointers of lhs and rhs. Calls lhs.swap(rhs).
	 * @param lhs, rhs - smart pointers whose contents to swap
	 */
	template<typename T>
	void swap( SharedPtr<T> &lhs, SharedPtr<T> &rhs )
	{
		return lhs.swap(rhs);
	}

	template< class T, class U > 
	bool operator==( const SharedPtr<T>& lhs, const SharedPtr<U>& rhs )
	{
		return lhs.get() == rhs.get();
	}

	template< class T, class U >
	bool operator!=( const SharedPtr<T>& lhs, const SharedPtr<U>& rhs )
	{
		return ! ( lhs == rhs );
	}

	/**
		* @fn template <class D, class T> D* get_deleter (const SharedPtr<T>& sp) throw()
		* @brief Get deleter from shared_ptr
		* If sp has no deleter, and thus would use ::delete to delete its managed object, the function returns a null pointer.
		* The returned value is valid at least as long as there exists a shared_ptr instance that owns that deleter.
		* Notice that the first template parameter is the return type, and thus cannot be automatically deduced by the compiler.
		*/
	template <class D, class T>
	D* get_deleter (const SharedPtr<T>& sp) throw()
	{
		return (sp.m_ctrl)? (& sp.m_ctrl.m_delCallback) : 0 ;
	}

	/**
		* @fn template <class T, class U> SharedPtr<T> static_SharedPtr_cast (const shared_ptr<U>& sp) throw();
		* @brief Static cast of SharedPtr
		* Returns a copy of sp of the proper type with its stored pointer casted statically from U* to T*.
		* If sp is not empty, the returned object shares ownership over sp's resources, increasing by one the use count.
		* If sp is empty, the returned object is an empty shared_ptr.
		* The function can only cast types for which the following expression would be valid: static_cast<T*>(sp.get())
		*/
	template <class T, class U>
	SharedPtr<T> static_SharedPtr_cast (const SharedPtr<U>& sp) throw()
	{
		//creating new smartPtr
		if (T* p = static_cast<T*>(sp.get()))
		{
			return SharedPtr<T> (sp, p );
		}
		else
		{
			return SharedPtr<T>();
		}
	}

	/**
		* @fn template <class T, class U> SharedPtr<T> dynamic_SharedPtr_cast (const shared_ptr<U>& sp) throw();
		* @brief Dynamic cast of SharedPtr
		* Returns a copy of sp of the proper type with its stored pointer casted dynamically from U* to T*.
		* If sp is not empty, and such a cast would not return a null pointer, the returned object shares ownership over sp's resources, increasing by one the use count.
		* Otherwise, the returned object is an empty shared_ptr.
		* The function can only cast types for which the following expression would be valid: dynamic_cast<T*>(sp.get())
		*/
	template <class T, class U>
	SharedPtr<T> dynamic_SharedPtr_cast (const SharedPtr<U>& sp) throw()
	{
		//creating new smartPtr
		if (T* p = dynamic_cast<T*>(sp.get()))
		{
			return SharedPtr<T> (sp, p );
		}
		else
		{
			return SharedPtr<T>();
		}
	}



} //memory
} //Core

#endif // __cplusplus

#endif// SHAREDPTR_HH
