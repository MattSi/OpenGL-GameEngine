#pragma once

class Object
{
	DECLARE_ABSTRACT_BASE_CLASS(Object)
protected:
	
	virtual void Initialize()=0;
	virtual void Destroy();
	virtual void Load(json::JSON& data);

public:
	bool IsInitialized() const { return initialized; }
	std::string& GetName() { return name; }
	int GetId() const { return id; }
	void SetIndent(int ind) { indent = ind; }
	int indent = 0;

	STRCODE GetId() { return uid; }
	void SetId(int _uid) { uid = _uid; }
	void SetName(const std::string& _name) { name = _name; }
private:
	bool initialized = false;
	std::string name = "Default Object";
	int id = -1;
	STRCODE uid = -1;
	std::string guid;
};

