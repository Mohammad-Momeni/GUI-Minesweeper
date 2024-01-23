#include "SBDL.h"
#include <ctime>
#include <fstream>
#include <iostream>
#include <climits>
#include <stdio.h>
#include <string.h>
#define window_width 500
#define window_height 500
using namespace std;
Font *Game_Font;
Texture Menu_Texture[6];
Texture Name_Texture;
SDL_Rect Game_Menu[6];
SDL_Rect Username={220,20,270,30};
SDL_Rect Name_Icon={100,20,120,30};
char Game_State='U'; // U for Username
struct Player{
    char Name[25] = {'\0'};
    int Point=0;
}My_Player;
struct Board{
    int Rows = 0;
    int Columns = 0;
    int Mines = 0;
}My_Game;
SDL_Rect Easy_Box={240,20,100,30};
SDL_Rect Medium_Box={240,60,100,30};
SDL_Rect Hard_Box={240,100,100,30};
SDL_Rect Custom_Box={240,140,100,30};
SDL_Rect EnterX_Box={240,20,100,30};
SDL_Rect EnterY_Box={240,60,100,30};
SDL_Rect EnterM_Box={240,100,100,30};
char Which_Box = 'X';
Texture Cells[12];
SDL_Rect NCells[25][25];
int Game_Board[25][25];
int Show_Board[25][25];
int Flags;
int Remaining_Choices;
char File_Name[20] = {'\0'};
bool isleaderboard = true;
void Initialization();
int String_Size(char Input[]);
void Address_Finder(char Temp[]);
bool isfile(char Address[]);
char* Name_Finder(string data);
int Point_Finder(string data);
int Line_Counter(string File_Name);
int Score_Finder(int Previous_Score);
char What_Key();
void Print_High(int Score,int& Printed,int& y);
void Enter_Name();
void Show_Menu();
void Leaderboard();
void Update_Leaderboard();
void Which_Board();
void Custom_Board();
void Cells_Initialization();
void Board_Initialization();
void Mines_Setup();
void New_Game();
char Choose_Flag(int x,int y);
char Choose_Select(int x,int y);
char Play_Rounds(int x,int y,int What_To_Do);
void Print_Map();
void Game_Process();
void Save_Game();
void Save_Process();
void Load_Game();
void Load_Process();
int main()
{

    SBDL::InitEngine("Minesweeper", window_width, window_height);
    Initialization();


    const int FPS = 60; //frame per second
    const int delay = 1000 / FPS; //delay we need at each frame



    while (SBDL::isRunning()) {
        int startTime = SBDL::getTime();

        SBDL::updateEvents();
        if(isleaderboard) {
            SBDL::clearRenderScreen();
        }


        //Game logic code


        if(Game_State == 'U'){ // U for username
            Enter_Name();
        }
        else if(Game_State == 'M'){ // M for menu
            Show_Menu();
        }
        else if(Game_State == 'W'){ // W for which board
            Which_Board();
        }
        else if(Game_State == 'C'){ // C for custom board
            Custom_Board();
        }
        else if(Game_State == 'O'){ // O for open game
            Load_Game();
        }
        else if(Game_State == 'G'){ // G for game process
            Game_Process();
        }
        else if(Game_State == 'L'){ // L for leaderboard
            Leaderboard();
        }
        else if(Game_State == 'S'){ // S for save menu
            Save_Game();
        }
        else if(Game_State == 'Q'){ // Q for quit
            break;
        }


        SBDL::updateRenderScreen();

        int elapsedTime = SBDL::getTime() - startTime;
        if (elapsedTime < delay)
            SBDL::delay(delay - elapsedTime);

    }
}
void Initialization()
{
    for(int i=0; i<12; i++) {
        Cells[i] = SBDL::loadTexture("assets/Cell" + to_string(i) + ".jpg");
    }
    Name_Texture= SBDL::loadTexture("assets/Username.png");

    Game_Font= SBDL::loadFont("assets/times.ttf",16);
    for(int i=0;i<6;i++){
        Menu_Texture[i]= SBDL::loadTexture("assets/Menu"+ to_string(i)+".png");
    }


    Game_Menu[0]={0,0,window_width,window_height};
    Game_Menu[1]={308,35,121,37};
    Game_Menu[2]={173,35,121,37};
    Game_Menu[3]={308,75,121,37};
    Game_Menu[4]={173,75,121,37};
    Game_Menu[5]={245,120,121,37};

}
int String_Size(char Input[])
{
    int i=0;
    while(Input[i]!='\0'){
        i++;
    }
    return i;
}
void Address_Finder(char Temp[])
{
    int Temp_Size= String_Size(File_Name);
    for(int i=5,j=0;j<Temp_Size;i++,j++) {
        Temp[i] = File_Name[j];
    }
}
bool isfile(char Address[])
{
    ifstream Test(Address);
    if(Test.is_open()) {
        Test.close();
        return true;
    }
    else {
        return false;
    }
}
char* Name_Finder(string data)
{
    int i=0;
    while(data[i] != ' ')
    {
        i++;
    }
    i++;
    char* Output = new char[30];
    int j=0;
    while(data[i] != '\0')
    {
        Output[j]=data[i];
        i++;
        j++;
    }
    Output[j] = '\0';
    return Output;
}
int Point_Finder(string data)
{
    char TPoint[200]={'\0'};
    int i=0;
    while(data[i]!=' ')
    {
        TPoint[i]=data[i];
        i++;
    }
    return atoi(TPoint);
}
int Line_Counter(string File_Name)
{
    int aNumOfLines = 0;
    ifstream aInputFile("assets/"+ File_Name);
    string aLineStr;
    while (getline(aInputFile, aLineStr)) {
        if (!aLineStr.empty()) {
            aNumOfLines++;
        }
    }
    return aNumOfLines;
}
int Score_Finder(int Previous_Score)
{
    int Player_Point;
    ifstream My_File("assets/Leaderboard");
    string data;
    getline(My_File, data);
    Player_Point = Point_Finder(data);
    int High_Score=-1;
    if(Player_Point < Previous_Score)
    {
        High_Score = Player_Point;
    }
    while(!My_File.eof())
    {
        getline(My_File, data);
        Player_Point = Point_Finder(data);
        if((High_Score < Player_Point) && (Player_Point < Previous_Score))
        {
            High_Score = Player_Point;
        }
    }
    My_File.close();
    return High_Score;
}
char What_Key()
{
    if((SBDL::keyPressed(SDL_SCANCODE_A))){
        return 'A';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_B))){
        return 'B';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_C))){
        return 'C';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_D))){
        return 'D';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_E))){
        return 'E';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_F))){
        return 'F';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_G))){
        return 'G';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_H))){
        return 'H';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_I))){
        return 'I';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_J))){
        return 'J';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_K))){
        return 'K';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_L))){
        return 'L';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_M))){
        return 'M';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_N))){
        return 'N';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_O))){
        return 'O';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_P))){
        return 'P';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_Q))){
        return 'Q';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_R))){
        return 'R';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_S))){
        return 'S';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_T))){
        return 'T';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_U))){
        return 'U';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_V))){
        return 'V';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_W))){
        return 'W';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_X))){
        return 'X';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_Y))){
        return 'Y';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_Z))){
        return 'Z';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_SPACE))){
        return ' ';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_0)) || (SBDL::keyPressed(SDL_SCANCODE_KP_0))){
        return '0';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_1)) || (SBDL::keyPressed(SDL_SCANCODE_KP_1))){
        return '1';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_2)) || (SBDL::keyPressed(SDL_SCANCODE_KP_2))){
        return '2';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_3)) || (SBDL::keyPressed(SDL_SCANCODE_KP_3))){
        return '3';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_4)) || (SBDL::keyPressed(SDL_SCANCODE_KP_4))){
        return '4';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_5)) || (SBDL::keyPressed(SDL_SCANCODE_KP_5))){
        return '5';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_6)) || (SBDL::keyPressed(SDL_SCANCODE_KP_6))){
        return '6';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_7)) || (SBDL::keyPressed(SDL_SCANCODE_KP_7))){
        return '7';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_8)) || (SBDL::keyPressed(SDL_SCANCODE_KP_8))){
        return '8';
    }
    else if((SBDL::keyPressed(SDL_SCANCODE_9)) || (SBDL::keyPressed(SDL_SCANCODE_KP_9))){
        return '9';
    }
}
void Print_High(int Score,int& Printed,int& y)
{
    int Player_Point;
    string Space="   ",Rate= to_string(Printed) + " :   ";
    Texture LB_Data;
    ifstream My_File("assets/Leaderboard");
    string data;
    getline(My_File, data);
    Player_Point = Point_Finder(data);
    if(Player_Point==Score)
    {
        LB_Data= SBDL::createFontTexture(Game_Font, Rate + Name_Finder(data) + Space + to_string(Player_Point),0,0,0);
        SBDL::showTexture(LB_Data,10,y);
        Printed++;
        y+=30;
    }
    else
    {
        while(!My_File.eof())
        {
            getline(My_File, data);
            Player_Point = Point_Finder(data);
            if(Score == Player_Point)
            {
                LB_Data= SBDL::createFontTexture(Game_Font,Rate + Name_Finder(data)+ Space + to_string(Player_Point),0,0,0);
                SBDL::showTexture(LB_Data,10,y);
                Printed++;
                y+=30;
            }
        }
    }
    My_File.close();
}
void Enter_Name()
{
    int Temp_Size= String_Size(My_Player.Name);

    SBDL::showTexture(Menu_Texture[0],Game_Menu[0]);
    SBDL::drawRectangle(Username,255,255,255);
    SBDL::showTexture(Name_Texture,Name_Icon);


    if(Temp_Size>0){
        Texture Temp_Name = SBDL::createFontTexture(Game_Font,My_Player.Name,0,0,0);
        SBDL::showTexture(Temp_Name,220,25);
    }


    if((SBDL::keyPressed(SDL_SCANCODE_RETURN) || SBDL::keyPressed(SDL_SCANCODE_KP_ENTER)) && Temp_Size> 0 && My_Player.Name[0] != ' ' && My_Player.Name[Temp_Size-1] != ' '){
        Update_Leaderboard();
        Game_State='M'; // M for Menu
    }
    if(Temp_Size<18){
        char What_Pressed = What_Key();
        if((What_Pressed >= 'A' && What_Pressed <= 'Z') || What_Pressed== ' ') {
            My_Player.Name[Temp_Size] = What_Pressed;
            My_Player.Name[Temp_Size+1] = '\0';
        }
    }
    if((SBDL::keyPressed(SDL_SCANCODE_BACKSPACE))){
        My_Player.Name[Temp_Size-1]='\0';
    }
}
void Show_Menu()
{
    for(int i=0;i<6;i++){
        SBDL::showTexture(Menu_Texture[i],Game_Menu[i]);
    }
    if(SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)&& SBDL::mouseInRect(Game_Menu[1])){
        Game_State='W'; // W for which board
    }
    else if(SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)&& SBDL::mouseInRect(Game_Menu[2])){
        File_Name[0] = '\0';
        Game_State='O'; // O for open game
    }
    else if(SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)&& SBDL::mouseInRect(Game_Menu[4])){
        Game_State='L'; // L for leaderboard
    }
    else if(SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)&& SBDL::mouseInRect(Game_Menu[3])){
        My_Player.Name[0] = '\0';
        My_Player.Point = 0;
        Game_State='U'; // U for username (changing username actually)
    }
    else if(SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)&& SBDL::mouseInRect(Game_Menu[5])){
        Game_State='Q'; // Q for quit
    }
}
void Leaderboard()
{
    if(isleaderboard) {
        int i=1,High;
        int File_Lines = Line_Counter("Leaderboard");
        int y=10;
        while(i<=File_Lines)
        {
            if(i==1)
            {
                High=Score_Finder(INT_MAX);
            }
            else
            {
                High=Score_Finder(High);
            }
            Print_High(High,i,y);
        }
        isleaderboard = false;
    }
    if(SBDL::keyPressed(SDL_SCANCODE_Q)) {
        isleaderboard = true;
        Game_State='M'; // M for menu
    }

}
void Update_Leaderboard()
{
    ofstream New_File("Temp");
    ifstream Old_File("assets/Leaderboard");
    string data;
    int File_Lines = Line_Counter("Leaderboard");
    for(int i=0; i<File_Lines; i++)
    {
        getline(Old_File,data);
        if(strcmp(Name_Finder(data),My_Player.Name) == 0)
        {
            My_Player.Point += Point_Finder(data);
        }
        else
        {
            New_File << data << endl;
        }
    }
    New_File << My_Player.Point << " " << My_Player.Name;

    New_File.close();
    Old_File.close();
    std::remove("assets/Leaderboard");
    std::rename("Temp" , "assets/Leaderboard");
}
void Which_Board()
{
    SBDL::showTexture(Menu_Texture[0],Game_Menu[0]);
    Texture Temp_Easy = SBDL::createFontTexture(Game_Font,"Easy",0,0,0);
    SBDL::showTexture(Temp_Easy,Easy_Box);
    Texture Temp_Medium = SBDL::createFontTexture(Game_Font,"Medium",0,0,0);
    SBDL::showTexture(Temp_Medium,Medium_Box);
    Texture Temp_Hard = SBDL::createFontTexture(Game_Font,"Hard",0,0,0);
    SBDL::showTexture(Temp_Hard,Hard_Box);
    Texture Temp_Custom = SBDL::createFontTexture(Game_Font,"Custom",0,0,0);
    SBDL::showTexture(Temp_Custom,Custom_Box);
    if(SBDL::mouseInRect(Easy_Box) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)) {
        My_Game.Rows = 5;
        My_Game.Columns = 5;
        My_Game.Mines = 4;
        New_Game();
        Game_State = 'G'; // G for game process
    }
    if(SBDL::mouseInRect(Medium_Box) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)) {
        My_Game.Rows = 12;
        My_Game.Columns = 12;
        My_Game.Mines = 28;
        New_Game();
        Game_State = 'G'; // G for game process
    }
    if(SBDL::mouseInRect(Hard_Box) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)) {
        My_Game.Rows = 20;
        My_Game.Columns = 20;
        My_Game.Mines = 96;
        New_Game();
        Game_State = 'G'; // G for game process
    }
    if(SBDL::mouseInRect(Custom_Box) && SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)) {
        Game_State = 'C'; // C for custom board
    }
    if(SBDL::keyPressed(SDL_SCANCODE_Q)) {
        Game_State = 'M'; // M for menu
    }
}
void Custom_Board()
{
    SBDL::showTexture(Menu_Texture[0],Game_Menu[0]);
    SBDL::drawRectangle(EnterX_Box,255,255,255);
    SBDL::drawRectangle(EnterY_Box,255,255,255);
    SBDL::drawRectangle(EnterM_Box,255,255,255);


    if(My_Game.Rows!=0) {
        Texture Temp_x = SBDL::createFontTexture(Game_Font,to_string(My_Game.Rows),0,0,0);
        SBDL::showTexture(Temp_x,240,25);
    }
    if(My_Game.Columns!=0) {
        Texture Temp_y = SBDL::createFontTexture(Game_Font,to_string(My_Game.Columns),0,0,0);
        SBDL::showTexture(Temp_y,240,65);
    }
    if(My_Game.Mines!=0) {
        Texture Temp_Mines = SBDL::createFontTexture(Game_Font,to_string(My_Game.Mines),0,0,0);
        SBDL::showTexture(Temp_Mines,240,105);
    }

    if(SBDL::keyPressed(SDL_SCANCODE_Q)) {
        My_Game.Rows = 0;
        My_Game.Columns = 0;
        My_Game.Mines = 0;
        Game_State = 'W'; // W for which board
    }

    char What_Pressed = What_Key();
    if(Which_Box == 'X') {
        if((What_Pressed >= '0' && What_Pressed <= '9') && (My_Game.Rows < 10) && (My_Game.Rows % 10 == 0 || My_Game.Rows % 10 == 1 || (My_Game.Rows % 10 == 2 && What_Pressed == '0'))) {
            int Temp = What_Pressed - '0';
            My_Game.Rows = (My_Game.Rows) * 10 + Temp;
        }
        if(SBDL::keyPressed(SDL_SCANCODE_BACKSPACE)) {
            My_Game.Rows /= 10;
        }
        if(SBDL::keyPressed(SDL_SCANCODE_RETURN) || SBDL::keyPressed(SDL_SCANCODE_KP_ENTER)) {
            if(My_Game.Rows >= 5) {
                Which_Box = 'Y';
            }
        }
    }
    else if(Which_Box == 'Y') {
        if((What_Pressed >= '0' && What_Pressed <= '9') && (My_Game.Columns < 10) && (My_Game.Columns % 10 == 0 || My_Game.Columns % 10 == 1 || (My_Game.Columns % 10 == 2 && What_Pressed == '0'))) {
            int Temp = What_Pressed - '0';
            My_Game.Columns = (My_Game.Columns) * 10 + Temp;
        }
        if(SBDL::keyPressed(SDL_SCANCODE_BACKSPACE)) {
            My_Game.Columns /= 10;
        }
        if(SBDL::keyPressed(SDL_SCANCODE_RETURN) || SBDL::keyPressed(SDL_SCANCODE_KP_ENTER)) {
            if(My_Game.Columns >= 5) {
                Which_Box = 'M';
            }
        }
    }
    else if(Which_Box == 'M') {
        if((What_Pressed >= '0' && What_Pressed <= '9') && (My_Game.Mines < 100)) {
            int Temp = What_Pressed - '0';
            My_Game.Mines = (My_Game.Mines) * 10 + Temp;
        }
        if(SBDL::keyPressed(SDL_SCANCODE_BACKSPACE)) {
            My_Game.Mines /= 10;
        }
        if(SBDL::keyPressed(SDL_SCANCODE_RETURN) || SBDL::keyPressed(SDL_SCANCODE_KP_ENTER)) {
            if((My_Game.Mines >= ((My_Game.Rows + My_Game.Columns) / 2) - 1) && (My_Game.Mines <= (My_Game.Rows * My_Game.Columns) - (My_Game.Rows + My_Game.Columns))) {
                Which_Box = 'X';
                New_Game();
                Game_State = 'G'; // G for game process
            }
        }
    }
}
void Cells_Initialization()
{
    int Temp_y = window_width / My_Game.Columns;
    int Temp_x = window_height / My_Game.Rows;

    if((My_Game.Rows == My_Game.Columns) && (Temp_x * My_Game.Columns == window_width) && (Temp_y * My_Game.Rows == window_height)) {
        int Cell_Size = window_height / My_Game.Rows;
        for(int i=1,x=0,y=0;i<=My_Game.Rows;i++,x=0,y+=Cell_Size) {
            for(int j=1;j<=My_Game.Columns;j++,x+=Cell_Size) {
                NCells[i][j]={x,y,Cell_Size,Cell_Size};
            }
        }
    }
    else {
        int Cell_Size = 25;
        int x,Temp;
        Temp = x = (window_width - (Cell_Size * My_Game.Columns)) / 2;
        int y = (window_height - (Cell_Size * My_Game.Rows)) / 2;
        for(int i=1;i<=My_Game.Rows;i++,x=Temp,y+=Cell_Size) {
            for(int j=1;j<=My_Game.Columns;j++,x+=Cell_Size) {
                NCells[i][j]={x,y,Cell_Size,Cell_Size};
            }
        }
    }
}
void Board_Initialization()
{
    for(int i=0;i<=My_Game.Rows+1;i++) {
        for(int j=0;j<=My_Game.Columns+1;j++){
            Show_Board[i][j] = -2;
            Game_Board[i][j] = 0;
        }
    }
}
void Mines_Setup()
{
    srand(time(NULL));
    for(int i=1;i<=My_Game.Mines;i++)
    {
        int Row = (rand() % My_Game.Rows) +1;
        int Column = (rand() % My_Game.Columns) +1;
        if(Game_Board[Row][Column] == 0)
        {
            Game_Board[Row][Column] = 1;
        }
        else
        {
            i--;
        }
    }
}
void New_Game()
{
    Cells_Initialization();
    Board_Initialization();
    Mines_Setup();
    Flags = 1;
    Remaining_Choices = (My_Game.Rows * My_Game.Columns) - My_Game.Mines;
}
char Choose_Flag(int x,int y)
{
    if(Show_Board[x][y]==-2) // -2 for not chosen yet
    {
        if(Flags<=My_Game.Mines)
        {
            Show_Board[x][y]= -3;
            Flags++;
            return 'F'; // F for flag placed
        }
        else
        {
            return 'N'; // N for max flag
        }
    }
    else if(Show_Board[x][y]==-3) // -3 for already have flag
    {
        Show_Board[x][y]=-2;
        Flags--;
        return 'R'; // R for flag removed
    }
}
char Choose_Select(int x,int y)
{
    if(Show_Board[x][y]==-2) // -2 for not chosen yet
    {
        if(Game_Board[x][y]==1) // 1 for mine
        {
            for(int i=1; i<=My_Game.Rows;i++)
            {
                for(int j=1; j<=My_Game.Columns;j++)
                {
                    if(Game_Board[i][j]==1)
                    {
                        Show_Board[i][j] =-1;
                    }
                }
            }
            return 'M'; // M for Mine
        }
        if(Game_Board[x][y]==0) // 0 for safe spot
        {
            Show_Board[x][y]= (Game_Board[x-1][y])+(Game_Board[x+1][y])+(Game_Board[x][y-1])+(Game_Board[x][y+1])+(Game_Board[x-1][y-1])+(Game_Board[x-1][y+1])+(Game_Board[x+1][y-1])+(Game_Board[x+1][y+1]);
            Remaining_Choices--;
            My_Player.Point++;
            if(Show_Board[x][y]==0)
            {
                for(int i=x-1;i<=x+1;i++)
                {
                    for(int j=y-1;j<=y+1;j++)
                    {
                        if(i>=1 && i<My_Game.Rows && j>=1 && j<=My_Game.Columns)
                        {
                            Choose_Select(i,j);
                        }
                    }
                }
            }
            return 'S'; // S for Safe spot
        }
    }
    else
    {
        return 'C'; // C for already chosen
    }
}
char Play_Rounds(int x,int y,int What_To_Do)
{
    if(What_To_Do==1) // 1 for select
    {
        return Choose_Select(x,y);
    }
    else // 2 for flag
    {
        return Choose_Flag(x,y);
    }
}
void Print_Map()
{
    for(int i=1;i<=My_Game.Rows;i++) {
        for(int j=1;j<=My_Game.Columns;j++){
            if(Show_Board[i][j] == -2) {
                SBDL::showTexture(Cells[9],NCells[i][j]);
            }
            else if(Show_Board[i][j] == -3) {
                SBDL::showTexture(Cells[10],NCells[i][j]);
            }
            else if(Show_Board[i][j] == -1) {
                SBDL::showTexture(Cells[11],NCells[i][j]);
            }
            else {
                int Temp = Show_Board[i][j];
                SBDL::showTexture(Cells[Temp],NCells[i][j]);
            }
        }
    }
}
void Game_Process()
{
    Print_Map();

    if(SBDL::Mouse.clicked(SDL_BUTTON_LEFT,1,SDL_PRESSED)){
        for(int i=1;i<=My_Game.Rows;i++){
            for(int j=1;j<=My_Game.Columns;j++){
                if(SBDL::mouseInRect(NCells[i][j])){
                    char Temp = Play_Rounds(i,j,1);
                    if(Temp == 'M') {
                        printf("You Lost\n");
                        Game_State = 'M';
                        My_Player.Point = 0;
                        My_Game.Rows = 0;
                        My_Game.Columns = 0;
                        My_Game.Mines = 0;
                    }
                }
            }
        }
    }
    if(SBDL::Mouse.clicked(SDL_BUTTON_RIGHT,1,SDL_PRESSED)){
        for(int i=1;i<=My_Game.Rows;i++){
            for(int j=1;j<=My_Game.Columns;j++){
                if(SBDL::mouseInRect(NCells[i][j])){
                    char Temp = Play_Rounds(i,j,2);
                }
            }
        }
    }


    if(Remaining_Choices == 0) {
        printf("You Won\n");
        Update_Leaderboard();
        Game_State = 'M';
        My_Player.Point = 0;
        My_Game.Rows = 0;
        My_Game.Columns = 0;
        My_Game.Mines = 0;
    }

    if(SBDL::keyPressed(SDL_SCANCODE_Q)) {
        Game_State = 'M'; // M for menu
        My_Player.Point = 0;
        My_Game.Rows = 0;
        My_Game.Columns = 0;
        My_Game.Mines = 0;
    }

    if(SBDL::keyPressed(SDL_SCANCODE_O)) {
        Game_State = 'S'; // S for save menu
        File_Name[0] = '\0';
    }
}
void Save_Game()
{
    int Temp_Size= String_Size(File_Name);

    SBDL::showTexture(Menu_Texture[0],Game_Menu[0]);
    SBDL::drawRectangle(Username,255,255,255);
    if(Temp_Size>0){
        Texture Temp_Name = SBDL::createFontTexture(Game_Font,File_Name,0,0,0);
        SBDL::showTexture(Temp_Name,220,25);
    }

    if((SBDL::keyPressed(SDL_SCANCODE_RETURN) || SBDL::keyPressed(SDL_SCANCODE_KP_ENTER)) && Temp_Size> 0 && File_Name[0] != ' ' && File_Name[Temp_Size-1] != ' '){
        Save_Process();
        Game_State='M'; // M for Menu
    }
    if(Temp_Size<18){
        char What_Pressed = What_Key();
        if((What_Pressed >= 'A' && What_Pressed <= 'Z') || What_Pressed== ' ') {
            File_Name[Temp_Size] = What_Pressed;
            File_Name[Temp_Size+1] = '\0';
        }
    }
    if((SBDL::keyPressed(SDL_SCANCODE_BACKSPACE))){
        File_Name[Temp_Size-1]='\0';
    }
}
void Save_Process()
{
    string Temp = File_Name;
    ofstream Saving("save/" + Temp);
    Saving << My_Player.Point << endl;
    Saving << My_Game.Rows << endl << My_Game.Columns << endl << My_Game.Mines << endl;
    Saving << Flags << endl << Remaining_Choices << endl;
    for(int i=0;i<=My_Game.Rows+1;i++) {
        for(int j=0;j<=My_Game.Columns+1;j++) {
            Saving << Game_Board[i][j];
            if(j!=My_Game.Columns+1) {
                Saving << " ";
            }
        }
        Saving << endl;
    }
    for(int i=0;i<=My_Game.Rows+1;i++) {
        for(int j=0;j<=My_Game.Columns+1;j++) {
            Saving << Show_Board[i][j];
            if(j!=My_Game.Columns+1) {
                Saving << " ";
            }
        }
        Saving << endl;
    }
    Saving.close();
    My_Player.Point = 0;
    My_Game.Rows = 0;
    My_Game.Columns = 0;
    My_Game.Mines = 0;
}
void Load_Game()
{
    int Temp_Size= String_Size(File_Name);

    SBDL::showTexture(Menu_Texture[0],Game_Menu[0]);
    SBDL::drawRectangle(Username,255,255,255);
    if(Temp_Size>0){
        Texture Temp_Name = SBDL::createFontTexture(Game_Font,File_Name,0,0,0);
        SBDL::showTexture(Temp_Name,220,25);
    }

    if((SBDL::keyPressed(SDL_SCANCODE_RETURN) || SBDL::keyPressed(SDL_SCANCODE_KP_ENTER)) && Temp_Size> 0 && File_Name[0] != ' ' && File_Name[Temp_Size-1] != ' '){
        char Temp[30] = "save/";
        Address_Finder(Temp);
        if(isfile(Temp)) {
            Load_Process();
            Game_State='G'; // G for game process
        }
    }
    if(Temp_Size<18){
        char What_Pressed = What_Key();
        if((What_Pressed >= 'A' && What_Pressed <= 'Z') || What_Pressed== ' ') {
            File_Name[Temp_Size] = What_Pressed;
            File_Name[Temp_Size+1] = '\0';
        }
    }
    if((SBDL::keyPressed(SDL_SCANCODE_BACKSPACE))){
        File_Name[Temp_Size-1]='\0';
    }
    if(SBDL::keyPressed(SDL_SCANCODE_DELETE)) {
        char Temp[30] = "save/";
        Address_Finder(Temp);
        std::remove(Temp);
        Game_State='M'; // M for Menu
    }
}
void Load_Process()
{
    char Temp[30] = "save/";
    Address_Finder(Temp);
    ifstream Loading(Temp);

    Loading >> My_Player.Point;
    Loading >> My_Game.Rows;
    Loading >> My_Game.Columns;
    Loading >> My_Game.Mines;
    Loading >> Flags;
    Loading >> Remaining_Choices;

    for(int i=0;i<=My_Game.Rows+1;i++) {
        for(int j=0;j<=My_Game.Columns+1;j++) {
            Loading >> Game_Board[i][j];
        }
    }
    for(int i=0;i<=My_Game.Rows+1;i++) {
        for(int j=0;j<=My_Game.Columns+1;j++) {
            Loading >> Show_Board[i][j];
        }
    }
    Cells_Initialization();

    Loading.close();
}