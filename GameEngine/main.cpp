#include "EngineCore.h"
#include "Engine.h"	
	  

int main(int argc, char* argv[]) {
	   
	    
	Engine& engine = Engine::GetInstance();
	      
	    
	engine.Initialize();
	engine.GameLoop();
	engine.Destroy();
	    
	return 0;
}                            