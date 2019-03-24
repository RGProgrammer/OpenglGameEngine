#include ".//ClassesDB.h"



void RGP_CORE::Class_DB::Clear()
{
	if (m_Classes) {
		for (_u32b i = 0; i < m_numClasses; ++i) {
			if (m_Classes[i].ptrMethods) {
				for (_u32b m = 0; m < m_Classes[i].numMethods; ++m) {
					if (m_Classes[i].ptrMethods[m].name)
						free(m_Classes[i].ptrMethods[m].name);
				}
				free(m_Classes[i].ptrMethods);
			}
			if (m_Classes[i].currentclassname)
				free(m_Classes[i].currentclassname);
		}
		free(m_Classes);
		m_Classes = NULL;
		m_numClasses = 0;
	}
};

_bool RGP_CORE::Class_DB::addClass(const _s8b* classname,void*(*createmethod)(void** args))
{
	if (find(classname)) {
		return false ;
	}
	_u32b num = m_numClasses;
	Class_DB_Element* tmp = (Class_DB_Element*)malloc((num+ 1) * sizeof(Class_DB_Element));
	
	if (!tmp)
		return false;
	for (_u32b i = 0; i < m_numClasses; ++i)
		tmp[i] = m_Classes[i];
	if (m_Classes) 	
		free(m_Classes);
	m_Classes = tmp;
	m_numClasses = ++num;
	m_Classes[num - 1].createInstanceMethod = createmethod;
	m_Classes[num - 1].currentclassname = CreateStringCopy((_s8b*)classname);
	m_Classes[num - 1].numMethods = 0;
	m_Classes[num - 1].ptrMethods = NULL;
	return true;
};
_bool RGP_CORE::Class_DB::addMethod(const _s8b* classname, const _s8b* methodname, void* pointer)
{
	if (!(classname && methodname && pointer))
		return false;
	_u32b index = find(classname);
	if (!index)
		return false;
	index--;//real index 
	
	Method* tmp = (Method*)malloc((m_Classes[index].numMethods + 1) * sizeof(Method));
	if (!tmp)
		return false;
	for (_u32b i = 0; i < m_Classes[index].numMethods; ++i)
		tmp[i] = m_Classes[index].ptrMethods[i];
	if (m_Classes[index].ptrMethods)
		free(m_Classes[index].ptrMethods);
	m_Classes[index].ptrMethods = tmp;
	m_Classes[index].numMethods++;
	m_Classes[index].ptrMethods[m_Classes[index].numMethods - 1].name = CreateStringCopy(methodname);
	m_Classes[index].ptrMethods[m_Classes[index].numMethods - 1].ptrMethod = pointer;
	return true;
};

void* RGP_CORE::Class_DB::getMethod(const _s8b* classname, const _s8b* methodname)
{
	void* pointer = NULL;
	_u32b index = find(classname);
	if (!index)
		return pointer;
	index--;
	for (_u32b i = 0; i < m_Classes[index].numMethods; ++i) {
		if (strcmp((_s8b*)methodname, m_Classes[index].ptrMethods[i].name) == 0) {
			pointer = m_Classes[index].ptrMethods[i].ptrMethod;
			break;
		}
	}
	return pointer ;
};
_u32b RGP_CORE::Class_DB::find(const _s8b* classname)
{
	_u32b index = 0;
	for (_u32b i = 0; i < m_numClasses; ++i) {
		if (strcmp((char*)classname, m_Classes[i].currentclassname) == 0) {
			index = i + 1;
			break;
		}
	}
	return index;
};

void*  RGP_CORE::Class_DB::getCreateMethod(const _s8b* classname)
{
	_u32b index = find(classname);
	if (index) {
		return m_Classes[index - 1].createInstanceMethod;
	}
	return NULL;
};
_u32b  RGP_CORE::Class_DB::getCreateMethodNumArgs(const _s8b* classname) {
	_u32b index = find(classname);
	if (index) {
		return m_Classes[index - 1].creatmethodNumArgs;
	}
	return 0;
};
