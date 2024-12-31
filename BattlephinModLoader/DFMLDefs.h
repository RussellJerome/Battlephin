#pragma once

//Stop being a fucking pussy c++ this will never be a actual issue for us
#pragma warning(disable : 4251)

//Easy To Manage, keeping it in one place.
#define MODLOADER_VERSION "1.0.0"

#ifdef BATTLEPHINMODLOADER_EXPORTS
#define LOADER_API __declspec(dllexport)
#else
#define LOADER_API __declspec(dllimport)
#endif