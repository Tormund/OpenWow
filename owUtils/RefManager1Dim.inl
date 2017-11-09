#pragma once

template <class OBJECT_TYPE>
inline OBJECT_TYPE* RefManager1Dim<OBJECT_TYPE>::Add(cstring name)
{
	OBJECT_TYPE* item;

	// If exists then return
	if ((item = GetItemByName(name)) != nullptr)
	{
		item->AddRef();
		return item;
	}

	// else create
	item = CreateAction(name);
	//if (item == nullptr)
	//{
	//	return nullptr;
	//}

	// Add item
	do_add(name, item);

	return item;
}

// Delete

template <class OBJECT_TYPE>
inline void RefManager1Dim<OBJECT_TYPE>::Delete(cstring name)
{
	OBJECT_TYPE* item = GetItemByName(name);
	if (item != nullptr)
	{
		item->DelRef();

		if (item->NeedDelete())
		{
			pre_delete(item);
		}
	}

}

template <class OBJECT_TYPE>
inline void RefManager1Dim<OBJECT_TYPE>::Delete(OBJECT_TYPE* item)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second == item)
		{
			this->Delete(it->first);
			return;
		}
	}
}

template <class OBJECT_TYPE>
inline void RefManager1Dim<OBJECT_TYPE>::DeleteAll()
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		pre_delete(it->second);
	}
}

// Getters

template <class OBJECT_TYPE>
inline OBJECT_TYPE* RefManager1Dim<OBJECT_TYPE>::GetItemByName(cstring name) const
{
	auto name_item = objects.find(name);
	if (name_item != objects.end())
	{
		return name_item->second;
	}

	return nullptr;
}

template <class OBJECT_TYPE>
inline std::string RefManager1Dim<OBJECT_TYPE>::GetNameByItem(OBJECT_TYPE* item) const
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		if (it->second == item)
		{
			return it->first;
		}
	}

	return "";
}

// Log

template <class OBJECT_TYPE>
inline void RefManager1Dim<OBJECT_TYPE>::PrintAllInfo()
{
	uint32 refsCnt = 0;
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		refsCnt += it->second->GetRefsCount();
		Log::Info("Item (%d) [%s]", it->second->GetRefsCount(), it->first.c_str());
	}

	Log::Info("Item's count [%d], items refs [%d]", objects.size(), refsCnt);
}

