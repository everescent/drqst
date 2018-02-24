//
//
//#include "Level_Editor.h"
//
//namespace
//{
//	static int ** Map_Array;
//	static int Map_Width;
//	static int Map_Height;
//
//
//
//	enum OBJ_TYPE
//	{
//		OBJ_TYPE_EMPTY, // 0
//		OBJ_PLATFORM,   // 1
//		OBJ_DRAGON,		// 2
//		OBJ_GRUNT,		// 3
//		OBJ_KING,		// 4
//		OBJ_MERLIN,		// 5
//		OBJ_LANCE,		// 6
//	};
//
//}
//
//int Import_MapData(char* filename)
//{
//	FILE * Exported_file;
//
//	errno_t error = (fopen_s(&Exported_file, filename, "r"));
//
//	if (error) return 0;
//
//	fscanf_s(Exported_file, "%*s %d", &Map_Width); // %*s is used to skip the first string character 
//	fscanf_s(Exported_file, "%*s %d", &Map_Height); //continue the scan for the height 
//	Map_Array = new int*[Map_Height];
//	for (int i = 0; i < Map_Height; ++i)
//	{
//		Map_Array[i] = new int [Map_Width];
//	}
//
//
//}
//
//void SnapToCell(float *Coordinate)
//{
//	int i = (int)*Coordinate;
//
//	*Coordinate = (float)i + 0.5f;
//}
//
//namespace Level_Editor
//{
//	void Init(void)
//	{
//
//	}
//
//	void Load(void)
//	{
//
//	}
//
//	void Update(float dt)
//	{
//
//	}
//
//	void Draw(void)
//	{
//
//	}
//
//	void Free(void)
//	{
//
//	}
//
//	void Unload(void)
//	{
//
//	}
//}
