#pragma once

template <typename T>
class Singleton
{
public:
	template <typename... Args>
	inline static T& GetInstance(Args&&... args)
	{

		if (instance == nullptr)
		{
			instance = new T();
			instance->Initialize(std::forward<Args>(args)...);			
		}
		return *instance;
	}

	inline static T& GetInstance()
	{	
		if (instance == nullptr)
		{
			return GetInstance<>();
		}
		
		return *instance;
	}

	inline void Destroy()
	{
		if (instance)
		{
			instance->CleanUp();
			delete instance;
			instance = nullptr;
		}
	}

	//virtual void Initialize() = 0;
	virtual void CleanUp() = 0;

private:
	static T* instance;

protected:
	inline explicit Singleton() = default;	
	virtual inline ~Singleton() = default;
	inline explicit Singleton(const Singleton&) = delete;
	inline Singleton& operator=(const Singleton&) = delete;
};
