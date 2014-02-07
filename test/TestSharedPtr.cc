//including only what we need to test
#include "UnitTest++/UnitTest++.h"
#include "Core/Memory/SharedPtr.hh"

//includes needed to run the test and write xml report
#include <iostream>
#include <fstream>
#include <cstring>
#include "UnitTest++/XmlTestReporter.h"

//to signal unimplemented test
bool NOT_IMPLEMENTED = false;

using Core::memory::SharedPtr;
using Core::memory::static_SharedPtr_cast;
using Core::memory::dynamic_SharedPtr_cast;

///>Testing generic SharedPtr with standard types
SUITE(SharedPtr)
{
		//custom type hierarchy to test inheritance, destruction, etc.
		typedef struct data_sample
		{
			virtual int gettestval() const {return 42;}
			virtual ~data_sample() {}
		}data_sample;
		typedef struct data_sample_child : data_sample
		{
			static bool destructor_called;
			data_sample_child() { destructor_called = false; } //to reinit at each new instanciation
			virtual int gettestval() const {return 4242;}
			virtual ~data_sample_child() { destructor_called = true; }
		}data_sample_child;
		bool data_sample_child::destructor_called = false;

		//testing default constructor with any type
		struct SharedPtr_default_bool_softinit //to test construction / destruction more easily
		{
			SharedPtr_default_bool_softinit() : TestPtr( 0 ) {}
			virtual void init()	{ TestPtr = new SharedPtr<bool>();	}
			virtual void destroy()	{ delete TestPtr; }
			virtual ~SharedPtr_default_bool_softinit() { TestPtr=0; }
			SharedPtr<bool>* TestPtr;
		};
		struct SharedPtr_default_bool : SharedPtr_default_bool_softinit
		{
			SharedPtr_default_bool() : SharedPtr_default_bool_softinit() { SharedPtr_default_bool_softinit::init(); }
			virtual ~SharedPtr_default_bool() { SharedPtr_default_bool_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { }
			virtual void destroy() {}
		};

		struct SharedPtr_default_int_softinit //to test construction / destruction more easily
		{
			SharedPtr_default_int_softinit() : TestPtr( 0 ) {}
			virtual void init()	{ TestPtr = new SharedPtr<int>(); }
			virtual void destroy()	{ delete TestPtr; }
			virtual ~SharedPtr_default_int_softinit() { TestPtr=0; }
			SharedPtr<int>* TestPtr;
		};
		struct SharedPtr_default_int : SharedPtr_default_int_softinit
		{
			SharedPtr_default_int() : SharedPtr_default_int_softinit() { SharedPtr_default_int_softinit::init(); }
			virtual ~SharedPtr_default_int() { SharedPtr_default_int_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { } 
			virtual void destroy() {}
		};

		struct SharedPtr_default_float_softinit
		{
			SharedPtr_default_float_softinit() : TestPtr( 0 ) {}
			virtual void init()	{ TestPtr = new SharedPtr<float>(); }
			virtual void destroy()	{ delete TestPtr; }
			virtual ~SharedPtr_default_float_softinit() { TestPtr=0; }
			SharedPtr<float>* TestPtr;
		};
		struct SharedPtr_default_float : SharedPtr_default_float_softinit
		{
			SharedPtr_default_float() : SharedPtr_default_float_softinit() { SharedPtr_default_float_softinit::init(); }
			virtual ~SharedPtr_default_float() { SharedPtr_default_float_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { } 
			virtual void destroy() {}
		};

		struct SharedPtr_default_data_sample_softinit
		{
			SharedPtr_default_data_sample_softinit() : TestPtr( 0 ) {}
			virtual void init()	{ TestPtr = new SharedPtr<data_sample>(); }
			virtual void destroy()	{ delete TestPtr; }
			virtual ~SharedPtr_default_data_sample_softinit() { TestPtr=0; }
			SharedPtr<data_sample>* TestPtr;
		};
		struct SharedPtr_default_data_sample : SharedPtr_default_data_sample_softinit
		{
			SharedPtr_default_data_sample() : SharedPtr_default_data_sample_softinit() { SharedPtr_default_data_sample_softinit::init(); }
			virtual ~SharedPtr_default_data_sample() { SharedPtr_default_data_sample_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { } 
			virtual void destroy() {}
		};

		template <class T>
		class check_delete
		{
		public:
			bool* p_deleted; //pointer needed, because deleter get copied into SharedPtr.
			check_delete(bool* deleted) : p_deleted (deleted) {}
			virtual void operator ()( T const * p) throw()
			{ 
				*p_deleted = true;
				delete p, p=0; //to cleanly delete the pointer
			}
		private:
			//forbidden because there is no meaning to it
			check_delete() : p_deleted (new bool(false)) {}
		};

		struct SharedPtr_bool_softinit
		{
			bool * testval; //untracked copy of pointer for testing
			SharedPtr_bool_softinit() : testval(new bool(true)),TestPtr(0) {}
			virtual void init() { TestPtr = new SharedPtr<const bool>( testval ); } //just for test. dont do that.
			template <class Deleter>
			void init(Deleter del) { TestPtr = new SharedPtr<const bool>( testval,del ); }//just for test. dont do that.
			virtual void destroy() { delete TestPtr,TestPtr=0; }
			virtual ~SharedPtr_bool_softinit() { if(testval && TestPtr) { delete testval, testval=0; } } //checking to avoid errors if already deleted by smart pointer
			SharedPtr<const bool>* TestPtr;
		};
		struct SharedPtr_bool : SharedPtr_bool_softinit
		{
			SharedPtr_bool() :SharedPtr_bool_softinit() { SharedPtr_bool_softinit::init(); }
			virtual ~SharedPtr_bool() { SharedPtr_bool_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { } 
			virtual void destroy() {}
		};

		struct SharedPtr_int_softinit
		{
			int* testval; //untracked copy of pointer for testing
			SharedPtr_int_softinit() : testval(new int(42)),TestPtr(0) {}
			virtual void init() { TestPtr = new SharedPtr<const int>( testval ); } //just for test. dont do that.
			template <class Deleter>
			void init(Deleter del) { TestPtr = new SharedPtr<const int>( testval, del ); }//just for test. dont do that.
			virtual void destroy() { delete TestPtr,TestPtr=0; }
			virtual ~SharedPtr_int_softinit() { if(testval && TestPtr) { delete testval, testval=0; } }//checking to avoid errors if already deleted by smart pointer
			SharedPtr<const int>* TestPtr;
		};
		struct SharedPtr_int : SharedPtr_int_softinit
		{
			SharedPtr_int() : SharedPtr_int_softinit() { SharedPtr_int_softinit::init(); }
			virtual ~SharedPtr_int() { SharedPtr_int_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { }
			virtual void destroy() {}
		};

		struct SharedPtr_float_softinit
		{
			float* testval; //untracked copy of pointer for testing
			SharedPtr_float_softinit() : testval(new float(42.f)),TestPtr(0) {}
			virtual void init() { TestPtr = new SharedPtr<const float>( testval ); }//just for test. dont do that.
			template <class Deleter>
			void init(Deleter del) { TestPtr = new SharedPtr<const float>( testval, del ); }//just for test. dont do that.
			virtual void destroy() { delete TestPtr,TestPtr=0; }
			virtual ~SharedPtr_float_softinit() { if(testval && TestPtr) { delete testval, testval=0; } }//checking to avoid errors if already deleted by smart pointer
			SharedPtr<const float>* TestPtr;
		};
		struct SharedPtr_float : SharedPtr_float_softinit
		{
			SharedPtr_float() : SharedPtr_float_softinit() { SharedPtr_float_softinit::init(); }
			virtual ~SharedPtr_float() { SharedPtr_float_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { }
			virtual void destroy() {}
		};

		struct SharedPtr_data_sample_softinit
		{
			data_sample* testval;
			SharedPtr_data_sample_softinit() : testval(new data_sample()),TestPtr( 0 ) {}
			virtual void init() { TestPtr = new SharedPtr<const data_sample>(testval); }//just for test. dont do that.
			template <class Deleter>
			void init(Deleter del) { TestPtr = new SharedPtr<const data_sample>(testval, del); }//just for test. dont do that.
			virtual void destroy() { delete TestPtr,TestPtr=0; }
			virtual ~SharedPtr_data_sample_softinit() { if(testval && TestPtr) { delete testval, testval=0; } }//checking to avoid errors if already deleted by smart pointer
			SharedPtr<const data_sample>* TestPtr;
		};
		struct SharedPtr_data_sample : SharedPtr_data_sample_softinit
		{
			SharedPtr_data_sample() : SharedPtr_data_sample_softinit() { SharedPtr_data_sample_softinit::init(); }
			virtual ~SharedPtr_data_sample() { SharedPtr_data_sample_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { }
			virtual void destroy() {}
		};

		struct SharedPtr_data_sample_child_softinit
		{
			data_sample_child* testval;
			SharedPtr_data_sample_child_softinit() : testval(new data_sample_child()),TestPtr( 0 ) {}
			virtual void init() { TestPtr = new SharedPtr<const data_sample>(testval); }//just for test. dont do that.
			template <class Deleter>
			void init(Deleter del) { TestPtr = new SharedPtr<const data_sample>(testval, del); }//just for test. dont do that.
			virtual void destroy() { delete TestPtr,TestPtr=0; }
			virtual ~SharedPtr_data_sample_child_softinit() { if(testval && TestPtr) { delete testval, testval=0; } }//checking to avoid errors if already deleted by smart pointer
			SharedPtr<const data_sample>* TestPtr;
		};
		struct SharedPtr_data_sample_child : SharedPtr_data_sample_child_softinit
		{
			SharedPtr_data_sample_child() : SharedPtr_data_sample_child_softinit() { SharedPtr_data_sample_child_softinit::init(); }
			virtual ~SharedPtr_data_sample_child() { SharedPtr_data_sample_child_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { }
			virtual void destroy() {}
		};

		struct SharedPtr_data_sample_child_smart_softinit
		{
			data_sample_child* testval;
			SharedPtr_data_sample_child_smart_softinit() : testval(new data_sample_child()),TestPtr( 0 ) {}
			virtual void init() { TestPtr = new SharedPtr<const data_sample_child>(testval); }//just for test. dont do that.
			template <class Deleter>
			void init(Deleter del) { TestPtr = new SharedPtr<const data_sample_child>(testval, del); }//just for test. dont do that.
			virtual void destroy() { delete TestPtr,TestPtr=0; }
			virtual ~SharedPtr_data_sample_child_smart_softinit() { if(testval && TestPtr) { delete testval, testval=0; } }//checking to avoid errors if already deleted by smart pointer
			SharedPtr<const data_sample_child>* TestPtr;
		};
		struct SharedPtr_data_sample_smart_child : SharedPtr_data_sample_child_smart_softinit
		{
			SharedPtr_data_sample_smart_child() : SharedPtr_data_sample_child_smart_softinit() { SharedPtr_data_sample_child_smart_softinit::init(); }
			virtual ~SharedPtr_data_sample_smart_child() { SharedPtr_data_sample_child_smart_softinit::destroy(); }
		private:
			//dont use this, really
			virtual void init() { }
			virtual void destroy() {}
		};

		///test default constructor and dereference operator
		TEST_FIXTURE(SharedPtr_default_bool_softinit,SharedPtr_default_bool_softinit)
		{
			init();

			CHECK ( TestPtr->IsNull() ); //default constructor creates and empty smart pointer

			CHECK ( TestPtr->use_count() == 0 );//The object is empty (owns no pointer, use count of zero).

			CHECK ( TestPtr->unique() == false );//Empty pointers are never unique (as they do not own any pointers).

			destroy();
		}

		TEST_FIXTURE(SharedPtr_default_int_softinit,SharedPtr_default_int_softinit)
		{
			init();

			CHECK ( TestPtr->IsNull() ); //default constructor creates and empty smart pointer
			
			CHECK ( TestPtr->use_count() == 0 );//The object is empty (owns no pointer, use count of zero).

			CHECK ( TestPtr->unique() == false );//Empty pointers are never unique (as they do not own any pointers).

			destroy();

		}

		TEST_FIXTURE(SharedPtr_default_float_softinit,SharedPtr_default_float_softinit)
		{
			init();

			CHECK ( TestPtr->IsNull() ); //default constructor creates and empty smart pointer
			
			CHECK ( TestPtr->use_count() == 0 );//The object is empty (owns no pointer, use count of zero).

			CHECK ( TestPtr->unique() == false );//Empty pointers are never unique (as they do not own any pointers).

			destroy();
		}

		TEST_FIXTURE(SharedPtr_default_data_sample_softinit,SharedPtr_default_data_sample_softinit)
		{
			init();

			CHECK ( TestPtr->IsNull() ); //default constructor creates and empty smart pointer
			
			CHECK ( TestPtr->use_count() == 0 );//The object is empty (owns no pointer, use count of zero).

			CHECK ( TestPtr->unique() == false );//Empty pointers are never unique (as they do not own any pointers).

			destroy();
		}

		///testing explicit constructor
		TEST_FIXTURE(SharedPtr_bool_softinit,SharedPtr_bool_softinit)
		{
			init();

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( *testval == *( *TestPtr ) );

			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();
		}

		TEST_FIXTURE(SharedPtr_int_softinit,SharedPtr_int_softinit)
		{
			init();

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( *testval == *( *TestPtr ) );
						
			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();
		}

		TEST_FIXTURE(SharedPtr_float_softinit,SharedPtr_float_softinit)
		{
			init();

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( *testval == *( *TestPtr ) );

			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();

		}

		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_softinit)
		{
			init();

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( testval->gettestval() == (*( *TestPtr ) ) . gettestval() );

			//checking other dereference operator
			CHECK ( testval->gettestval() == ( *TestPtr ) -> gettestval() );

			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();

		}

		///testing destructor via custom deleter
		TEST_FIXTURE(SharedPtr_bool_softinit,SharedPtr_bool_softinit_delete)
		{
			bool deleter_called = false;
			check_delete<bool> testdeleter(&deleter_called);
			init(testdeleter);

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( *testval == *( *TestPtr ) );

			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();

			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_int_softinit,SharedPtr_int_softinit_delete)
		{
			bool deleter_called = false;
			check_delete<int> testdeleter(&deleter_called);
			init(testdeleter);

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( *testval == *( *TestPtr ) );
						
			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();

			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_float_softinit,SharedPtr_float_softinit_delete)
		{
			bool deleter_called = false;
			check_delete<float> testdeleter(&deleter_called);
			init(testdeleter);

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( *testval == *( *TestPtr ) );

			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();

			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_softinit_delete)
		{
			bool deleter_called = false;
			check_delete<data_sample> testdeleter(&deleter_called);
			init(testdeleter);

			//checking constructor is working as expected
			CHECK ( testval == (*TestPtr).get() );

			//checking dereference operator
			CHECK ( testval->gettestval() == (*( *TestPtr ) ) . gettestval() );

			//checking other dereference operator
			CHECK ( testval->gettestval() == ( *TestPtr ) -> gettestval() );

			CHECK ( TestPtr->IsNull() == false );
			
			CHECK ( TestPtr->use_count() == 1 );

			CHECK ( TestPtr->unique() );

			destroy();
			
			CHECK (deleter_called);
		}

		///test copy constructor and destructor
		TEST_FIXTURE(SharedPtr_bool_softinit,SharedPtr_bool_copy)
		{
			bool deleter_called = false;
			check_delete<bool> testdeleter(&deleter_called);
			init(testdeleter);

			{
				SharedPtr<const bool> copyPtr (*TestPtr); 

				//checking dereference operator
				CHECK ( *copyPtr == *( *TestPtr ) );
	
				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
				
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
				
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_int_softinit,SharedPtr_int_copy)
		{
			bool deleter_called = false;
			check_delete<int> testdeleter(&deleter_called);
			init(testdeleter);

			{
				SharedPtr<const int> copyPtr (*TestPtr); 

				//checking dereference operator
				CHECK ( *copyPtr == *( *TestPtr ) );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_float_softinit,SharedPtr_float_copy)
		{
			bool deleter_called = false;
			check_delete<float> testdeleter(&deleter_called);
			init(testdeleter);

			{
				SharedPtr<const float> copyPtr (*TestPtr); 

				//checking dereference operator
				CHECK ( *copyPtr == *( *TestPtr ) );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);

		}

		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_copy)
		{
			bool deleter_called = false;
			check_delete<data_sample> testdeleter(&deleter_called);
			init(testdeleter);

			{
				//implicit conversion here (const data_sample to data_sample) to use specialized copy constructor
				SharedPtr<const data_sample> copyPtr (*TestPtr); 

				//checking dereference operator
				CHECK ( copyPtr->gettestval() == ( *TestPtr ) -> gettestval() );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}
		
		///test aliasing constructor and destructor
		TEST_FIXTURE(SharedPtr_bool_softinit,SharedPtr_bool_alias)
		{
			bool deleter_called = false;
			check_delete<bool> testdeleter(&deleter_called);
			init(testdeleter);

			bool aliasval = true;
			{
				SharedPtr<const bool> copyPtr (*TestPtr,&aliasval);

				//checking dereference operator
				CHECK ( *copyPtr == aliasval );
	
				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
				
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
				
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_int_softinit,SharedPtr_int_alias)
		{
			bool deleter_called = false;
			check_delete<int> testdeleter(&deleter_called);
			init(testdeleter);
			
			int aliasval = -42;
			{
				SharedPtr<const int> copyPtr (*TestPtr,&aliasval); 

				//checking dereference operator
				CHECK ( *copyPtr == aliasval );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_float_softinit,SharedPtr_float_alias)
		{
			bool deleter_called = false;
			check_delete<float> testdeleter(&deleter_called);
			init(testdeleter);

			float aliasval = -42.f;
			{
				SharedPtr<const float> copyPtr (*TestPtr,&aliasval); 

				//checking dereference operator
				CHECK ( *copyPtr == aliasval );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);

		}

		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_alias)
		{
			bool deleter_called = false;
			check_delete<data_sample> testdeleter(&deleter_called);
			init(testdeleter);

			data_sample aliasval; //default constructor is enough here
			{
				SharedPtr<const data_sample> copyPtr (*TestPtr,&aliasval); 

				//checking dereference operator
				CHECK ( copyPtr->gettestval() == aliasval.gettestval() );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( copyPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( copyPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( copyPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		///test assignement operator and destructor
		TEST_FIXTURE(SharedPtr_bool_softinit,SharedPtr_bool_assign)
		{
			bool deleter_called = false;
			check_delete<bool> testdeleter(&deleter_called);
			init(testdeleter);

			{
				SharedPtr<const bool> assignPtr = (*TestPtr); 

				//checking dereference operator
				CHECK ( *assignPtr == *( *TestPtr ) );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( assignPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( assignPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( assignPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_int_softinit,SharedPtr_int_assign)
		{
			bool deleter_called = false;
			check_delete<int> testdeleter(&deleter_called);
			init(testdeleter);

			{
				SharedPtr<const int> assignPtr = (*TestPtr); 

				//checking dereference operator
				CHECK ( *assignPtr == *( *TestPtr ) );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( assignPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( assignPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( assignPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_float_softinit,SharedPtr_float_assign)
		{
			bool deleter_called = false;
			check_delete<float> testdeleter(&deleter_called);
			init(testdeleter);

			{
				SharedPtr<const float> assignPtr = (*TestPtr); 

				//checking dereference operator
				CHECK ( *assignPtr == *( *TestPtr ) );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( assignPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( assignPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( assignPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_assign)
		{
			bool deleter_called = false;
			check_delete<data_sample> testdeleter(&deleter_called);
			init(testdeleter);

			{
				SharedPtr<const data_sample> assignPtr = (*TestPtr); 

				//checking dereference operator
				CHECK ( assignPtr->gettestval() == ( *TestPtr ) -> gettestval() );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( assignPtr.IsNull() == false );
			
				CHECK ( TestPtr->use_count() == 2 );
				CHECK ( assignPtr.use_count() == 2 );
			
				CHECK ( TestPtr->unique() == false );
				CHECK ( assignPtr.unique() == false );
			}
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			CHECK (deleter_called);

		}

		///swap and correct destructor behavior with inheritance
		TEST_FIXTURE(SharedPtr_data_sample_child_softinit,SharedPtr_data_sample_child_swap)
		{
			init();
			{
				SharedPtr<const data_sample> swapPtr( new data_sample() );

				//checking dereference operator
				CHECK ( swapPtr->gettestval() == 42 );
				CHECK ( ( *TestPtr ) -> gettestval() == 4242 );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( TestPtr->use_count() == 1 );
				CHECK ( TestPtr->unique() == true );

				CHECK ( swapPtr.IsNull() == false );
				CHECK ( swapPtr.use_count() == 1 );
				CHECK ( swapPtr.unique() == true );

				//swapping
				TestPtr->swap(swapPtr);

				//checking again
				CHECK ( swapPtr->gettestval() == 4242 );
				CHECK ( ( *TestPtr ) -> gettestval() == 42 );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( TestPtr->use_count() == 1 );
				CHECK ( TestPtr->unique() == true );

				CHECK ( swapPtr.IsNull() == false );
				CHECK ( swapPtr.use_count() == 1 );
				CHECK ( swapPtr.unique() == true );

			}
			
			CHECK ( data_sample_child::destructor_called );

			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();//data_sample destructor called
		}

		///swap symmetrical check
		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_swap)
		{
			init();
			{
				SharedPtr<const data_sample> swapPtr( new data_sample_child() );

				//checking dereference operator
				CHECK ( swapPtr->gettestval() == 4242 );
				CHECK ( ( *TestPtr ) -> gettestval() == 42 );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( TestPtr->use_count() == 1 );
				CHECK ( TestPtr->unique() == true );

				CHECK ( swapPtr.IsNull() == false );
				CHECK ( swapPtr.use_count() == 1 );
				CHECK ( swapPtr.unique() == true );

				//swapping
				TestPtr->swap(swapPtr);

				//checking again
				CHECK ( swapPtr->gettestval() == 42 );
				CHECK ( ( *TestPtr ) -> gettestval() == 4242 );

				CHECK ( TestPtr->IsNull() == false );
				CHECK ( TestPtr->use_count() == 1 );
				CHECK ( TestPtr->unique() == true );

				CHECK ( swapPtr.IsNull() == false );
				CHECK ( swapPtr.use_count() == 1 );
				CHECK ( swapPtr.unique() == true );

			} //data_sample destructor called
			
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();

			CHECK ( data_sample_child::destructor_called );

		}
		
		///reset null
		TEST_FIXTURE(SharedPtr_bool_softinit, SharedPtr_bool_reset_null)
		{
			init();
			//checking pointer value after init
			CHECK ( *testval == *( *TestPtr ) );

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			TestPtr->reset();

			//here pointer has been deallocated
			CHECK ( TestPtr->IsNull() == true );
			CHECK ( TestPtr->use_count() == 0 );
			CHECK ( TestPtr->unique() == false );

			//this has no effect, as pointer is empty
			destroy();
		}

		TEST_FIXTURE(SharedPtr_int_softinit, SharedPtr_int_reset_null)
		{
			init();
			//checking pointer value after init
			CHECK ( *testval == *( *TestPtr ) );

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			TestPtr->reset();

			//here pointer has been deallocated
			CHECK ( TestPtr->IsNull() == true );
			CHECK ( TestPtr->use_count() == 0 );
			CHECK ( TestPtr->unique() == false );

			//this has no effect, as pointer is empty
			destroy();
		}

		TEST_FIXTURE(SharedPtr_float_softinit, SharedPtr_float_reset_null)
		{
			init();
			//checking pointer value after init
			CHECK ( *testval == *( *TestPtr ) );

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			TestPtr->reset();

			//here pointer has been deallocated
			CHECK ( TestPtr->IsNull() == true );
			CHECK ( TestPtr->use_count() == 0 );
			CHECK ( TestPtr->unique() == false );

			//this has no effect, as pointer is empty
			destroy();
		}

		TEST_FIXTURE(SharedPtr_data_sample_softinit, SharedPtr_data_sample_reset_null)
		{
			init();

			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 42 );

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			TestPtr->reset();

			//here pointer has been deallocated
			CHECK ( TestPtr->IsNull() == true );
			CHECK ( TestPtr->use_count() == 0 );
			CHECK ( TestPtr->unique() == false );

			//this has no effect, as pointer is empty
			destroy();
		}

		///reset with smartptr to basic types and custom delete to check destructor
		TEST_FIXTURE(SharedPtr_bool_softinit, SharedPtr_bool_reset)
		{
			bool deleter_called = false;
			check_delete<bool> testdeleter(&deleter_called);
			// deleter on init
			init(testdeleter);

			//checking pointer value after init
			CHECK ( *testval == *( *TestPtr ) );

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			TestPtr->reset(new bool(false));
			
			//custom destructor passed on init called here
			CHECK (deleter_called);

			//here pointer is still unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
		}

		TEST_FIXTURE(SharedPtr_int_softinit, SharedPtr_int_reset)
		{
			bool deleter_called = false;
			check_delete<int> testdeleter(&deleter_called);
			//no deleter on init
			init();

			//checking pointer value after init
			CHECK ( *testval == *( *TestPtr ) );

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			TestPtr->reset(new int(-42), testdeleter );

			//here pointer is still unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
			
			//custom destructor passed on reset called here
			CHECK (deleter_called);
		}

		TEST_FIXTURE(SharedPtr_float_softinit, SharedPtr_float_reset)
		{
			bool deleter_called = false;
			check_delete<float> testdeleter(&deleter_called);
			//no deleter on init
			init();

			//checking pointer value after init
			CHECK ( *testval == *( *TestPtr ) );

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			TestPtr->reset(new float(-42.f), testdeleter);

			//here pointer is still unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			destroy();
						
			//custom destructor passed on reset called here
			CHECK (deleter_called);
		}

		///reset and correct destructor behavior with inheritance
		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_reset)
		{
			bool deleter_called = false;
			check_delete<data_sample> testdeleter(&deleter_called);
			//no deleter on init
			init();

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );
			
			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 42 );

			TestPtr->reset( new data_sample_child(), testdeleter );

			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 4242 );

			destroy();

			//custom destructor passed on reset called here
			CHECK (deleter_called);
			CHECK ( data_sample_child::destructor_called );
		}

		///symmetrical reset and correct destructor behavior with inheritance
		TEST_FIXTURE(SharedPtr_data_sample_child_softinit,SharedPtr_data_sample_child_reset)
		{
			bool deleter_called = false;
			check_delete<data_sample> testdeleter(&deleter_called);
			//no deleter on init
			init();
			
			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );
			
			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 4242 );

			TestPtr->reset( new data_sample(), testdeleter );

			//correct destructor called on reset
			CHECK ( data_sample_child::destructor_called );

			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );

			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 42 );

			destroy();

			//custom destructor passed on reset called here
			CHECK (deleter_called);

		}

		///casting operators
		TEST_FIXTURE(SharedPtr_data_sample_child_smart_softinit,SharedPtr_data_sample_child_smart_static_cast)
		{
			init();
			
			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );
			
			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 4242 );

			//static up casting
			SharedPtr<const data_sample> castPtr = static_SharedPtr_cast<const data_sample> ( *TestPtr );

			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 2 );
			CHECK ( TestPtr->unique() == false );

			CHECK ( castPtr.IsNull() == false );
			CHECK ( castPtr.use_count() == 2 );
			CHECK ( castPtr.unique() == false );

			//checking dereference operator on casted pointer
			CHECK ( castPtr->gettestval() == 4242 ); //polymorphism still working as expected

			//destructor still not called ( to avoid false negative on next check )
			CHECK ( false == data_sample_child::destructor_called );

			destroy();
			
			//destructor not called ( still another instance exists )
			CHECK ( false == data_sample_child::destructor_called );

			castPtr.reset();

			//correct destructor called
			CHECK ( true == data_sample_child::destructor_called );

		}

		TEST_FIXTURE(SharedPtr_data_sample_child_softinit,SharedPtr_data_sample_child_dynamic_cast_ok)
		{
			init();
			
			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );
			
			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 4242 ); //polymorphism is ok

			//dynamic down casting
			SharedPtr<const data_sample_child> castPtr = dynamic_SharedPtr_cast<const data_sample_child> ( *TestPtr );

			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 2 );
			CHECK ( TestPtr->unique() == false );

			CHECK ( castPtr.IsNull() == false );
			CHECK ( castPtr.use_count() == 2 );
			CHECK ( castPtr.unique() == false );

			//checking dereference operator on casted pointer
			CHECK ( castPtr->gettestval() == 4242 );

			//destructor still not called ( to avoid false negative on next check )
			CHECK ( false == data_sample_child::destructor_called );

			castPtr.reset();
			
			//destructor not called ( another instance still exist )
			CHECK ( false == data_sample_child::destructor_called );

			destroy();

			//correct destructor called
			CHECK ( true == data_sample_child::destructor_called );
		}

		TEST_FIXTURE(SharedPtr_data_sample_softinit,SharedPtr_data_sample_dynamic_cast_fail)
		{
			init();
			
			//initializing flag to check it is not modified ( we dont construct child in this test, so it is not already reinitialized to false )
			data_sample_child::destructor_called=false;

			//here pointer is unique
			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() );
			
			//checking dereference operator
			CHECK ( ( *TestPtr ) -> gettestval() == 42 ); //polymorphism is not ok : this is not a data_sample_child instance

			//dynamic down casting ( should fail )
			SharedPtr<const data_sample_child> castPtr = dynamic_SharedPtr_cast<const data_sample_child> ( *TestPtr );

			CHECK ( TestPtr->IsNull() == false );
			CHECK ( TestPtr->use_count() == 1 );
			CHECK ( TestPtr->unique() == true );

			//castPtr should be empty since casting failed
			CHECK ( castPtr.IsNull() == true );
			CHECK ( castPtr.use_count() == 0 );
			CHECK ( castPtr.unique() == false );
						
			//destructor still not called ( to avoid false negative on next check )
			CHECK ( false == data_sample_child::destructor_called );

			//this shouldnt do anything
			castPtr.reset();
			
			//child destructor not called (castPtr is empty pointer)
			CHECK ( false == data_sample_child::destructor_called );

			destroy();

			//child destructor not called ( not a child instance )
			CHECK ( false == data_sample_child::destructor_called );
		}

}

int main(int argc, char * argv [])
{
	if ( argc < 2 )
	{
		return UnitTest::RunAllTests();
	}
	else if ( strcmp(argv[1],"-xml") == 0 )
	{
		std::ofstream f("UnitTest-Util-result.xml");
		UnitTest::XmlTestReporter reporter(f);
		UnitTest::TestRunner runner(reporter);
		return runner.RunTestsIf(UnitTest::Test::GetTestList(),NULL,UnitTest::True(),0);
	}
	else
	{
		std::cerr << "Usage : " << argv[0] << " [-xml]" << std::endl;
		return 127;
	}
}