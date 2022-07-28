# NFC/Sensor Support on Matrix Creator Board
## This project combines the use of the built in NFC and sensors on the Matrix creator board so that scanning an NFC tag will gather data from the sensors

### <span style="color: Orange"> Required Hardware: </span>&ensp;Raspberry Pi Model 3 B +
### &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;Matrix Creator Board 
### <span style="color: Orange"> Required Software: </span> &nbsp;&nbsp;&nbsp;Raspbian Buster OS
### &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;Python 3.7.3
### &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&ensp;g++ (Raspbian 8.3.0-6+rpi1) 8.3.0
### <span style="color: Green"> Optional Hardware: </span> &ensp;Extension cable for the PI GPIO pins so it doesn't affect the temperature sensor      

<br/>


### <span style="color: Yellow"> Setup: </span>
- To setup your pi to use the matrix boards sensors as well as NFC go to https://matrix-io.github.io/matrix-documentation/matrix-hal/getting-started/. You need to download matrix hal either from package or from source. Download "from package" didn't work for me so I used "from source" which worked as well as provided useful examples that I used in my code.

- Then download hal nfc from https://matrix-io.github.io/matrix-documentation/matrix-hal/getting-started/installation-nfc/. And follow their entire procedure. 

- Then try to compile their examples. If it doesn't work for you, you have to go to your libraries and check if all the libraries downloaded properly in the RIGHT SPOT. They need to be downloaded in usr/local/include/\*. For me it was instead downloaded in one of the other folders usr/\* folders. Move the files to the user/local/include\* to make it compile.

- Now copy the clone the files from the github repository into your desired folder. You MUST change the #include folder directories to where you have python, everloop, nfc and drivers (.h files) saved.

- Then you can run the file after compiling the cpp program using. To compile use <span style="color: Green"> g++ -o combinedSensorsNFC combinedSensorsNFC.cpp -std=c++11 -DNXPBUILD__PH_RASPBERRY_PI -I/usr/local/include/matrix_nfc/nxp_nfc/NxpNfcRdLib/types -I/usr/local/include/matrix_nfc/nxp_nfc/NxpNfcRdLib/intfs -lmatrix_hal_nfc -lmatrix_creator_hal -I/usr/include/python3.7 -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -L/usr/lib -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions
</span> 
Note: U might need to change some of the paths in the compile command.

- There is no need to change anything in the python program. A text file will be created that the python file will read and its all handles itself.

#### <span style="color: Orange"> Useful commands: </span>

To compile the C++ program: 
- g++ -o combinedSensorsNFC combinedSensorsNFC.cpp -std=c++11 -DNXPBUILD__PH_RASPBERRY_PI -I/usr/local/include/matrix_nfc/nxp_nfc/NxpNfcRdLib/types -I/usr/local/include/matrix_nfc/nxp_nfc/NxpNfcRdLib/intfs -lmatrix_hal_nfc -lmatrix_creator_hal -I/usr/include/python3.7 -L/usr/lib/python3.7/config-3.7m-arm-linux-gnueabihf -L/usr/lib -lpython3.7m -lcrypt -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions
- Might need to change the path of the python call

To run the program
- ./combinedSensorsNFC

To exit the program
- ctrl+z
- ps
- kill -9 "PID#" don't use quotes. The PID number will be displayed by the ps command. 

#### <span style="color: Orange"> Trouble Shooting: </span> 
- The matrix board will often take a few seconds to start up its NFC reader. This might require you to recompile. 
- If the board still doesn't work try using a different matrix board.
- If you are getting errors during compiling check where your libraries are stored. Follow the path in the #include on the C++ program and ensure there is a library there

Many optimizations can be made to the code. The cpp program can be made to exit cleanly. In espeak you can use the -f command to read a file instead of reading it in line by line like I did. It was read in line by line because it made it easier to analyize each line as the voice messed up certain words.

A better method of using python can also be used but I used \#include <python.h> as it was the easiest
