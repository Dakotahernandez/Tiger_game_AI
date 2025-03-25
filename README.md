# Tiger_game_AI
The goal of the projct is to create an AI that can play the tiger game. 
Using a given API to pick moves and using SDL to visualize


# Details about the Tiger game 
Overview 
The tiger game is a traditional Chinese board game. It is a strategy game for two players. One player, who plays the tiger, has only one piece. The other player has 18 men. The tiger must eat the men, who must block the tiger so that he cannot move. The game can be played with any kind of pieces that can be distinguished from one another. The board is 8×8 squares, with the addition of a square of 2×2 squares, which is called the tiger's lair.

Play
The men cannot take the tiger, but can only block him. The tiger, by jumping over a man, can take it, in which case the man is removed from play. The men must stay together 2 by 2 to block the tiger. In general, if a tiger eats more than three pieces, it is unlikely that men will have enough pieces to win the game. If the men do not make mistakes, but move cautiously, victory is likely.

# The play area 

<img width="206" alt="Screenshot 2025-03-25 at 8 10 59 AM" src="https://github.com/user-attachments/assets/72bf8088-5549-4a49-8b0e-dd6243df5e92" />

# Guide to getting SDL_2 to work on xcode for Mac(working on mac silicon and intel chips)
The given libary also requires SDL_MIXER

1)if you dont have homebrew download in your terminal using the following command <br />

/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)
<br /> <br />

2)download SDL2 and the mixer in your terminal using homebrew use the following command <br />
brew install sdl2 sdl2_mixer  <br /> <br />


3)Create a new xcode project:set as a command line tool using c++ <br /> <br />


4)Link SDL2 and SDL2_mixer Binary Libraries in Xcode <br />
You need to link the SDL2 and SDL2_mixer binary libraries to your Xcode project. <br />
In Xcode, select your project from the Project Navigator (left panel). <br />
Under Targets, select your project target (usually the same name as your project). <br />
Go to the Build Phases tab. <br />
In the Link Binary With Libraries section, click the + button. <br />
Choose Add Other <br />
once the tab comes up use the keybind command + shift + G <br />
then search  <br /> 
/opt/homebrew/opt/sdl2_mixer/lib/    <br /> 
/opt/homebrew/opt/sdl2/lib    <br /> 
Add libSDL2_mixer-2.0.0.dylib and libSDL2-2.0.0.dylib to your project(it will say open instead of add but it will add) <br /><br />


5)Set Up Header Search Paths <br />
Select your project in the Project Navigator (left-hand side). <br />
Go to the Build Settings tab. <br />
In the search bar, type Header Search Paths. <br />
Double-click on Header Search Paths and add the following paths: <br />
Add debug and release <br />
For SDL2: /opt/homebrew/include  <br />
For SDL2_mixer: /opt/homebrew/include/SDL2 <br />
leave both as non recursive <br /> <br />



6)Set Up Library Search Paths <br />
In Build Settings, search for Library Search Paths. <br />
if it says inherited you dont need to add anything leave it as is <br />
Double-click Library Search Paths and add  <br />
For SDL2: /opt/homebrew/include  <br />
For SDL2_mixer: /opt/homebrew/include/SDL2 <br />
non recursive again <br /> <br />


7)finally theres been a recent update to the SDL libraies so their signauture is diffrent <br />
and flagged by apple security so you must turn off signature checking  <br />
Select your project in the Project Navigator (the left panel). <br />
Go to the "Signing & Capabilities" Tab: <br />
Add the "Disable Library Validation" Entitlement: <br />
Scroll down to see if the "App Sandbox" capability is enabled.  <br />
If not, add it by clicking the + button in the upper-left corner and selecting App Sandbox. <br />
Once App Sandbox is added, click on it to expand the entitlement options. <br />
Under Security, you’ll find the Disable Library Validation option. <br />
Enable it by checking the box next to Disable Library Validation. <br />
 <br />
the SDL is now properlly connected <br />
copy paste SDL_test_run.txt into your main
now clean your build (shift + command + k) <br />
try and build (shift + command + b)it before running to try and make sure there are no error <br />
if it builds safely run it(command + r) <br /> <br />


additional notes  <br />
if you cant find your paths for the search paths you can search for your search paths in your console with  <br />
find / -name "SDL2" 2>/dev/null <br />
