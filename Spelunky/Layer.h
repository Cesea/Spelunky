#ifndef LAYER_H
#define LAYER_H

#include <vector>

template <typename T>
class Layer
{
public :
	Layer() {};
	virtual ~Layer() {};

	void Init()
	{
		_objects.reserve(30);
	}
	void Release()
	{
		for (auto &object : _objects)
		{
			(object)->Release();
			delete object;
		}
		_objects.clear();
	}

	inline void PushObject(T t)
	{
		_objects.push_back(t);
	}

	inline void PushObjectVector(std::vector<T> objectsVector)
	{
		_objects.insert(_objects.end(), objectsVector.begin(), objectsVector.end());
	}

	inline std::vector<T> &GetObjectsVector()
	{
		return _objects;
	}

	inline T GetObjectIndexOf(int num)
	{
		return _objects[num];
	}

	void RemoveObject(T t)
	{
		for (auto _objectsIter = _objects.begin(); _objectsIter != _objects.end();)
		{
			if ((*_objectsIter) == t)
			{
				_objectsIter = _objects.erase(_objectsIter);
				break;
			}
			else
				_objectsIter++;
		}
	}

	void RemoveObjectIfEnd()
	{
		for (auto _objectsIter = _objects.begin(); _objectsIter != _objects.end();)
		{
			if ((*_objectsIter)->GetRenderData()->GetAnimateSprites().size() > 0 && 
				((*_objectsIter)->GetRenderData()->GetAnimateSprites()[0]->IsEnd()))
			{
				delete (*_objectsIter);
				_objectsIter = _objects.erase(_objectsIter);
				break;
			}
			else
				_objectsIter++;
		}
	}

	void RemoveObjectIfDead()
	{
		for (auto _objectsIter = _objects.begin(); _objectsIter != _objects.end();)
		{
			if ((*_objectsIter)->IsDead() )
			{
				delete (*_objectsIter);
				_objectsIter = _objects.erase(_objectsIter);
				break;
			}
			else
				_objectsIter++;
		}
	}

private :
	std::vector<T > _objects;
//	std::vector<T >::iterator _objectsIter;
};

#endif
//
//template<typename T>
//inline void Layer<T>::Init()
//{
//	_objects.reserve(30);
//}
//
//template<typename T>
//inline void Layer<T>::Release()
//{
//	for (auto &object : _objects)
//	{
//		(object)->Release();
//		delete object;
//	}
//	_objects.clear();
//}
//
//template<typename T>
//inline void Layer<T>::RemoveObject(T t)
//{
//	for (auto _objectsIter = _objects.begin(); _objectsIter != _objects.end();)
//	{
//		if ((*_objectsIter) == t)
//		{
//			_objectsIter = _objects.erase(_objectsIter);
//		}
//		else
//			_objectsIter++;
//	}
//
//}
