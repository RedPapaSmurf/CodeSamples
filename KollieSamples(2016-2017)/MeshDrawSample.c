//This is an example of the mesh creation function I wrote to make
//sqare meshes

	// 1 triangle at a time
	// X, Y, Color, texU, texV
	AddTriangleToMesh(
		0.0f, 0.0f, 0xFFFF0000, 0.0f, 0.0f,
		mapDraw->squareWidth, 0.0f, 0xFFFF0000, 0.25f, 0.0f,
		0.0f, mapDraw->squareHeight, 0xFFFF0000, 0.0f, 0.25f);

	AddTriangleToMesh(
		mapDraw->squareWidth, 0.0f, 0xFFFF0000, 0.25f, 0.0f,
		mapDraw->squareWidth, mapDraw->squareHeight, 0xFFFF0000, 0.25f, 0.25f,
		0.0f, mapDraw->squareHeight, 0xFFFF0000, 0.0f, 0.25f);

	AE_ASSERT_MESG(mapDraw->pMapMesh, "Failed to create mesh 1!!");
