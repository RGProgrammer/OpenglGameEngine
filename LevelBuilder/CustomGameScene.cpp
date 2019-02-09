#include "./CustomGameScene.h"

RGP_LEVELBUILDER::CustomScene::CustomScene() :GameScene(), m_isSelected(NULL), m_NumElements(0)
{
}

RGP_LEVELBUILDER::CustomScene::~CustomScene()
{
	this->Destroy();
}

void RGP_LEVELBUILDER::CustomScene::Destroy()
{
	GameScene::Destroy();
	if (m_isSelected) {
		free(m_isSelected);
		m_isSelected = NULL;
		m_NumElements = 0;
	}
	
}

_bool RGP_LEVELBUILDER::CustomScene::isActorSelected(_u32b index)
{
	if (index >= m_NumElements)
		return false;
	if (m_isSelected[index])
		return true;
	else return false;
}

_bool RGP_LEVELBUILDER::CustomScene::isActorSelected(const _s8b * actorName)
{
	for (_u32b i = 0; i < getNumActors() && i<m_NumElements; ++i) {
		if (strcmp(actorName, this->getActor(i)->getName()) == 0) {
			return m_isSelected[i];
		}
	}
	return false;
}

_bool RGP_LEVELBUILDER::CustomScene::setActorAsSelected(_u32b index, _bool Selected)
{
	if (index >= m_NumElements)
		return false;
	m_isSelected[index] = Selected;
	return true;

}

_bool RGP_LEVELBUILDER::CustomScene::setActorAsSelected(const _s8b * actorName, _bool Selected)
{
	for (_u32b i = 0; i < getNumActors(); ++i) {
		if (strcmp(actorName, this->getActor(i)->getName()) == 0) {
			 m_isSelected[i] = Selected;
			 return true;
		}
	}
	return false;
}
_bool* RGP_LEVELBUILDER::CustomScene::getMemoryCase(_u32b index)
{
	if (index < m_NumElements)
		return &m_isSelected[index];
	return  NULL;
};

_u32b RGP_LEVELBUILDER::CustomScene::AddActor(BaseActor * actor)
{
	_bool*  tmp = NULL;
	if (GameScene::AddActor(actor)) {
			tmp = (_bool*)malloc(getNumActors() * sizeof(_bool));
			if (!tmp) {
				GameScene::RemoveActor(actor);
				return 0;
			}
			for (_u32b i = 0; i < m_NumElements; ++i) {
				tmp[i] = m_isSelected[i];
			}
			free(m_isSelected);
			m_isSelected = tmp;

		m_isSelected[getNumActors() - 1] = false;
		m_NumElements = getNumActors();
		return  getNumActors();
	}
	return 0;
}


_bool RGP_LEVELBUILDER::CustomScene::RemoveActorAt(_u32b index)
{
	if (GameScene::RemoveActorAt(index)) {

		for (_u32b i = index; i < m_NumElements - 1; ++i) {
			m_isSelected[i] = m_isSelected[i + 1];
		}
		return true;
	}
	return false;
}

_bool RGP_LEVELBUILDER::CustomScene::RemoveActor(BaseActor * actor)
{
	_u32b index = 0;
	for (_u32b i = 0; i < m_NumElements; ++i) {
		if (this->getActor(i) == actor) {
			index = i;
		}
	}
	if (GameScene::RemoveActor(actor)) {
		for (_u32b i = index; i < m_NumElements-1; ++i) {
			m_isSelected[i] = m_isSelected[i + 1];
		}
		return true;
	}
	return false;
}

void RGP_LEVELBUILDER::CustomScene::RemoveSelectedActors()
{
	for (_u32b i = 0; i < this->getNumActors();) {
		if (this->isActorSelected(i)) {
			this->RemoveActorAt(i);
		}
		else {
			++i;
		}
	}
}
