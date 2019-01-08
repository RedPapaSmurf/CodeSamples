//This was my first hands on experience with mallocing data which taught me a lot.
//This code parses a game file and sends it off to be built in engine.


		int(*currLevel2)[NUM_OF_SQUARES_DOWN * NUM_OF_SQUARES_ACROSS];
		currLevel2 = malloc(sizeof(int[NUM_OF_SQUARES_DOWN * NUM_OF_SQUARES_ACROSS]));


		while (fgets(line, NUM_OF_SQUARES_ACROSS + 10, fp))
		{
			int lineLength = strlen(line);
			// starts at 0, increments at loop end rowint = 0 ++;
			//loop through each element of the line and give coordinates

			// Due to less than equals instead of less than, we write one full row
			// of integers into memory we do not own
			// do not assume the line contains NUM_OF_SQUARES_ACROSS of VALID values.
			// AKA the case that a line looks like this:
			// 7777
			// \/ BAD CODE
			// for (currCol = 0; currCol <= NUM_OF_SQUARES_ACROSS; currCol++)

			// minus one so that we don't process the newline at the end.
			for (currCol = 0; currCol < lineLength - 1; currCol++)
			{
				// starts at 0, increments at loop end colint = 0 ++
				if (line[currCol] == 'p')
				{
					AEVec2 pos = { (float)currCol, (float)currRow };
					currNum = -1;
					playerCreate(pos);
				}
				if ((line[currCol] >= '0') && (line[currCol] <= '9'))
					currNum = line[currCol] - '0' - 1;
				else if ((line[currCol] >= 'A') && (line[currCol] <= 'Z'))
					currNum = line[currCol] - 'A' - 1 + 10;

   				(*currLevel)[currRow * NUM_OF_SQUARES_ACROSS + currCol] = currNum;
				(*currLevel2)[currRow * NUM_OF_SQUARES_ACROSS + currCol] = currNum;
			}
			currRow++;
		}
