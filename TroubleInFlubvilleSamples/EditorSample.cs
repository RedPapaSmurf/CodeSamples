//This file was written in C# to latch into our editor.
//here I invoke "texconv.exe" and convert the provided file into a .DDS
//file with the specified command line arguments.
...
            Process texConverterProcess = new Process();
            texConverterProcess.StartInfo.FileName = "Assets/Util/TexConv/texconv.exe";

            string ddsDirectory = spriteToConvert.Remove(spriteToConvert.LastIndexOf("\\"), spriteToConvert.Length - spriteToConvert.LastIndexOf("\\"));
            //UnityEngine.Debug.Log(ddsDirectory);
            // If the directory doesn't exist create it
            System.IO.Directory.CreateDirectory(Application.dataPath + "\\..\\..\\" + ddsDirectory + "\\");

            // If the DDS file already exists then don't convert
            if (System.IO.File.Exists(Application.dataPath + "\\..\\..\\" + ddsDirectory + "\\" + GetFileName(spriteToConvert) + ".DDS"))
            {
                return ddsDirectory + "\\";
            }

            // texconv args, including output path and the sprite to convert
            texConverterProcess.StartInfo.Arguments = "-m 3 -pow2 -f BC2_UNORM -o ..\\" + ddsDirectory + "\\ " + spriteToConvert; // Sprite png to convert

            texConverterProcess.StartInfo.UseShellExecute = false;
            texConverterProcess.StartInfo.RedirectStandardOutput = true;

            // Run the DDS converter
            texConverterProcess.Start();
            texConverterProcess.WaitForExit();\
...
