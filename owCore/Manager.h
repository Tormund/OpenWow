#pragma once
/*
#include <string>
#include <map>
using std::string;
using std::map;


class T { };

class RefManager {
public:
	void Add(cstring _name, T _object) {
		if(objects.find(_name) != objects.end()) {
			(*references.find(_name)).second++;
		}
		else {
			objects.insert(make_pair(_name, _object));
			references.insert(make_pair(_name, 0));
		}
	}

	void Delete(T _object) {
		auto refsIt = references.find()
		if()

		unsigned id = 0xFFFFFFFF;
		unsigned refsCount = 0;

		// Find object
		auto idsIt = ids.find(_name);
		if(idsIt != ids.end())
			id = (*idsIt).second;
		else
			return;

		// Refs
		auto refsIt = references.find(id);
		if(refsIt != references.end()) {
			if(--((*refsIt).second) == 0) {

				objects.erase()

			}
		}
		else
			return;



		ManagedItem* i = items[_object];
		DeleteAction(_object);
		names.erase(names.find(i->name));
		items.erase(items.find(_object));
		delete i;
	}

	void Delete(cstring _name) {
		if(Exists(_name))
			Delete(Get(_name));
	}

	//

	virtual void CreateAction(T id) = 0;
	virtual void DeleteAction(T id) = 0;

	//

	bool Exists(cstring name) final {
		return names.find(name) != names.end();
	}

	T Get(cstring name) final {
		return names[name];
	}

private:
	map<string, T> objects;              // object name - object
	map<string, unsigned> references;    // object name - reference count
};

//

class IntManager : public RefManager<int> {
	int baseid;
public:
	IntManager() : baseid(0) { }

protected:
	int GetNextID() {
		return baseid++;
	}
};*/