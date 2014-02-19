/**
 * Copyright (c) 2009-2014, Asmodehn's Corp.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *	    this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *		notice, this list of conditions and the following disclaimer in the
 *	    documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Asmodehn's Corp. nor the names of its
 *	    contributors may be used to endorse or promote products derived
 *	    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * contact : alex@asmodehn.com
 */


//from Boost v 1.55

#ifndef __CORE_UTILS_VALUE_INIT_HH
#define __CORE_UTILS_VALUE_INIT_HH

#include <type_traits>

namespace Core
{
	namespace Utils
	{

		template<class T>
		class initialized
		{
		private :
			struct wrapper
			{
			  typename std::remove_const<T>::type data;
			  wrapper() : data()
			  {}

			  wrapper(T const & arg) : data(arg)
			  {}
			};

		mutable
		  typename std::aligned_storage<sizeof(wrapper), std::alignment_of<wrapper>::value>::type x;

		wrapper * wrapper_address() const
		{
		  return static_cast<wrapper *>( static_cast<void*>(&x));
		}

		public :
			initialized() : x() {}
			explicit initialized(T const & arg) : x(arg) {}
			operator T const &() const;
			operator T&();
			T const &data() const;
			T& data();
			void swap( initialized& );
		} ;

		template<class T>
		T const& get ( initialized<T> const& x );

		template<class T>
		T& get ( initialized<T>& x );

		template<class T>
		void swap ( initialized<T>& lhs, initialized<T>& rhs );


		template<class T>
		class value_initialized
		{

		public :

			value_initialized() : x() {}
			operator T const &() const { return x ; }
			operator T&() { return x ; }
			T const &data() const { return x ; }
			T& data() { return x ; }
			void swap( value_initialized& );

		private :
			initialized<T> x ;

		} ;
		
		template<class T>
		T const& get ( value_initialized<T> const& x )
		{
			return x.data() ;
		}

		template<class T>
		T& get ( value_initialized<T>& x )
		{
			return x.data() ;
		}

		template<class T>
		void swap ( value_initialized<T>& lhs, value_initialized<T>& rhs )
		{
			lhs.swap(rhs) ;
		}


		class initialized_value_t
		{
		public :
			template <class T> operator T() const ;
		};

		initialized_value_t const initialized_value = {} ;

	}; //namespace Utils

}; //namespace Core


#endif // __CORE_UTILS_VALUE_INIT_HH
