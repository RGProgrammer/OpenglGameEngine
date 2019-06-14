#ifndef _RGP_CLASSES_DB_H_
#define _RGP_CLASSES_DB_H_
#include "..//Common//BasePrimitiveTypes.h"
#include "..//Common//Common.h"
namespace RGP_CORE {
	
	

	namespace Class_DB {
		typedef struct {
			char* name;
			void* ptrMethod;
		} Method;
		typedef struct{
			char*		currentclassname;
			Method*     ptrMethods;
			_u32b		numMethods;
			void*       (*createInstanceMethod)(void** args);
			_u32b		creatmethodNumArgs;
		} Class_DB_Element;


		
		 static Class_DB_Element*			m_Classes=NULL;
		 static _u32b						m_numClasses= 0;
		
		 _u32b find(const _s8b* classname);

		 void		Clear();
		 _bool addClass(const _s8b* classname, void*(*createmethod)(void** args) =NULL );
		 _bool addMethod(const _s8b* classname, const _s8b* methodname, void* pointer);
		 void* getMethod(const _s8b* classname, const _s8b* methodname);
		 void* getCreateMethod(const _s8b* classname);
		 _u32b getCreateMethodNumArgs(const _s8b* classname);
		

	};
}

#endif // !_RGP_CLASSES_DB_H_
