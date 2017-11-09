#pragma once

#include <map>

template <class OBJECT_TYPE, class ID_TYPE>
inline OBJECT_TYPE* RefManager2Dim<OBJECT_TYPE, ID_TYPE>::Add(cstring name)
{
	ID_TYPE id;

	// If exists
	if ((id = GetIDByName(name)) != 0)
	{
		items[id]->AddRef();
		//Log::Warn("Item [%s] already exists", name.c_str());
		return items[id];
	}

	// Generate ID
	id = GenerateID();

	// Create item
	OBJECT_TYPE* item = CreateAction(name, id);
	//if (item == nullptr)
	//{
	//	return nullptr;
	//}

	// Add item
	do_add(name, id, item);

	return item;
}

// Delete

template <class OBJECT_TYPE, class ID_TYPE>
inline void RefManager2Dim<OBJECT_TYPE, ID_TYPE>::Delete(cstring name)
{
	std::map<string, ID_TYPE>::iterator name_id = names.find(name);
	if (name_id != names.end())
	{
		Delete(name_id->second);
	}
}

template <class OBJECT_TYPE, class ID_TYPE>
inline void RefManager2Dim<OBJECT_TYPE, ID_TYPE>::Delete(ID_TYPE id)
{
	OBJECT_TYPE* item = GetItemByID(id);
	if (item != nullptr)
	{

		item->DelRef();

		if (item->NeedDelete())
		{
			pre_delete(id, item);
		}
	}
}

template <class OBJECT_TYPE, class ID_TYPE>
inline void RefManager2Dim<OBJECT_TYPE, ID_TYPE>::Delete(OBJECT_TYPE* item)
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		if (it->second == item)
		{
			Delete(it->first);
			return;
		}
	}
}

template <class OBJECT_TYPE, class ID_TYPE>
inline void RefManager2Dim<OBJECT_TYPE, ID_TYPE>::DeleteAll()
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		pre_delete(it->first, it->second);
	}
}

// Getters

template <class OBJECT_TYPE, class ID_TYPE>
inline ID_TYPE RefManager2Dim<OBJECT_TYPE, ID_TYPE>::GetIDByName(cstring name)
{
	std::map<string, ID_TYPE>::iterator name_id = names.find(name);
	if (name_id != names.end())
	{
		return name_id->second;
	}

	return 0;
}

template <class OBJECT_TYPE, class ID_TYPE>
inline OBJECT_TYPE* RefManager2Dim<OBJECT_TYPE, ID_TYPE>::GetItemByID(ID_TYPE id)
{
	std::map<ID_TYPE, OBJECT_TYPE*>::iterator id_item = items.find(id);
	if (id_item != items.end())
	{
		return id_item->second;
	}

	return nullptr;
}

template <class OBJECT_TYPE, class ID_TYPE>
inline OBJECT_TYPE* RefManager2Dim<OBJECT_TYPE, ID_TYPE>::GetItemByName(cstring name)
{
	OBJECT_TYPE* item = nullptr;

	ID_TYPE id;
	if ((id = GetIDByName(name)) == 0)
		return nullptr;

	return GetItemByID(id)
}

template <class OBJECT_TYPE, class ID_TYPE>
inline void RefManager2Dim<OBJECT_TYPE, ID_TYPE>::PrintAllInfo()
{
	uint32 refsCnt = 0;
	for (auto it = names.begin(); it != names.end(); ++it)
	{
		OBJECT_TYPE* item = GetItemByID(it->second);
		refsCnt += item->GetRefsCount();
		Log::Info("Item (%d) [%d]-[%s]", item->GetRefsCount(), it->second, it->first.c_str());
	}

	Log::Info("Item's count [%d], items refs [%d]", names.size(), refsCnt);
}

