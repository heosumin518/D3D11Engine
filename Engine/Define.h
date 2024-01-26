#pragma once

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()

#define CHECK(p)	assert(SUCCEEDED(p))
#define GAME		GET_SINGLE(Game)
#define GRAPHICS	GET_SINGLE(Graphics)
#define DEVICE		GRAPHICS->GetDevice()
#define DC			GRAPHICS->GetDeviceContext()
#define TIME		GET_SINGLE(TimeManager)
#define DT			TIME->GetDeltaTime()
#define RESOURCES	GET_SINGLE(ResourceManager)
#define RENDER		GET_SINGLE(RenderManager)


#define LOG_MESSAGEA(...) \
{ \
    char buffer[256]; \
    sprintf_s(buffer, 256, "[MESSAGE] %s:%d - ", __FUNCTION__, __LINE__); \
    char message[256]; \
    sprintf_s(message, 256, __VA_ARGS__); \
    strcat_s(buffer, message); \
    strcat_s(buffer, "\n"); \
    OutputDebugStringA(buffer); \
}