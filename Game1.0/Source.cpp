#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<math.h>
#include "map.h"
#include<iostream>
#include<sstream>
#include<cassert>
using namespace sf;
using namespace std;

RenderWindow window(VideoMode(1280, 800), "SFML works!");

class Player_Ball {
public:
    double tempX, tempY;
    int PROVERKA = 0;
    int WIN = 1; //переменная для запуска различных карт, когда мяч попадает в зону действия объекта (переход уровня)
    int chek_X = 0;
    int sum = 0;
    double speed=0.0;
    double position_X, position_Y;
    double Direction_X=0, Direction_Y=0;

    void SetStartPosition(double position_X, double position_Y) { //метод ,который задаёт начальные координаты мяча на поле(Нужен для того, что у нас много уровне и везде свой спавнпоинт)
        this->position_X = position_X;
        this->position_Y = position_Y;
    }

    void SetValues() { //Если человек проиграл, то все значения равны 0
        speed = 0.0;
        Direction_X = 0.0;
        Direction_Y = 0.0;
    }
};

class All_Sprite {
public: 
    Image ball, background, music_icon, next_button, reset;
    Texture T_ball, T_background, T_music_icon, T_next_button, T_reset;
    Sprite S_ball, S_background, S_music_icon, S_next_button, S_reset;

    All_Sprite() {
        ball.loadFromFile("ball11.png");
        background.loadFromFile("background.png");
        music_icon.loadFromFile("music_icon.png");
        next_button.loadFromFile("playbutton.png");
        reset.loadFromFile("reset.png");
        T_ball.loadFromImage(ball);
        T_background.loadFromImage(background);
        T_music_icon.loadFromImage(music_icon);
        T_next_button.loadFromImage(next_button);
        T_ball.loadFromImage(ball);
        T_background.loadFromImage(background);
        T_music_icon.loadFromImage(music_icon);
        T_next_button.loadFromImage(next_button);
        T_reset.loadFromImage(reset);
        S_ball.setTexture(T_ball);
        S_background.setTexture(T_background);
        S_music_icon.setTexture(T_music_icon);
        S_next_button.setTexture(T_next_button);
        S_reset.setTexture(T_reset);
        // Включаем режим сглаживания для нашей текстуры
        T_ball.setSmooth(true);
    }
};

class HIT {
public:
    Music hit;
    void Hit() {
        hit.openFromFile("hit.ogg");
        hit.play();
        hit.setVolume(30.f);
    }
};

// Очень странно, что игровое поле является наследником игрового мяча
class Playing_Field : public Player_Ball, public HIT , public All_Sprite {
public:
    int chek_Y = 0, time_to_complete_the_level[3] = {0 , 1250 , 500}, time_to_win=0;
    bool lazer_on_off = true, chekswamp = false;

    void ContactObject_WithAnBall(String* NumMap, int i, int j) { //проверка мяча на спорикосновение со всем объектами на карте в радиусе одного квадрата

        if ((NumMap[i + 1][j+1] == 's' || NumMap[i+1][j] == 's' || NumMap[i][j] == 's' ||  NumMap[i][j + 1] == 's' || NumMap[i + 1][j + 1] == '0' || NumMap[i + 1][j] == '0' || NumMap[i][j + 1] == '0' || NumMap[i][j] == '0')) { //если символ равен 's' (камень) или '0' (стена)
             

                position_X += Direction_X;//идем по иксу с помощью вектора нормали
                position_Y += -Direction_Y;//идем по игреку так же

                
                if (position_Y>=i*32 + 32 || position_Y<=i*32) { // test -----
                    if (position_X <=32){
                        position_X += -Direction_X * 2;//идем по иксу с помощью вектора нормали
                        position_Y += Direction_Y * 2;//идем по игреку так же
                        PROVERKA = 2;
                        Hit();
                        chek_Y = 0;
                    }
                    chek_X++;
                    sum = 2;
                    Hit();
                    chek_Y = 0;
                    PROVERKA = 1;
                } //-------------
                else  {
                    position_X += -Direction_X*2;//идем по иксу с помощью вектора нормали
                    position_Y += Direction_Y*2;//идем по игреку так же
                    PROVERKA = 2;
                    Hit();
                    chek_Y = 0;
                }
            
            time_to_win++;
            cout << endl << "hit Y poz :";
            cout << "X:" << j << " Y:" << i << " " << "xBoll:" << position_X << " yBoll:" << position_Y << " " << " | chek right side " << sum << "| ";
        }
        //------------------------------------ 

        if (NumMap[i + 1][j] == 'w' || NumMap[i][j] == 'w' || NumMap[i][j + 1] == 'w' || NumMap[i+1][j+1] == 'w') {
            cout << "You WIN";
            if(time_to_win < time_to_complete_the_level[WIN-1] || time_to_complete_the_level[WIN-1] == 0) // первое условие для уровней на время, второй для вступительных уровней, чтобы пользователь смог понять
            WIN++; //меняет карту на другую
            switch (WIN)
            {
            case 1: SetStartPosition(1120.0, 704.0); break;
            case 2:SetStartPosition(160.0, 96.0); break;
            case 3:SetStartPosition(50.0, 50.0); break;
            }
            SetValues();
            time_to_win = 0;
        }

        //-------------------------------------

        if ((NumMap[i + 1][j] == 'k' || NumMap[i][j] == 'k' || NumMap[i][j + 1] == 'k' || NumMap[i + 1][j + 1] == 'k') && lazer_on_off == true || (NumMap[i + 1][j] == '#' || NumMap[i][j] == '#' ) || (NumMap[i + 1][j] == 'l' || NumMap[i][j] == 'l' || NumMap[i][j + 1] == 'l' || NumMap[i + 1][j + 1] == 'l') || ((NumMap[i + 1][j] == 'K' || NumMap[i][j] == 'K' || NumMap[i][j + 1] == 'K' || NumMap[i + 1][j + 1] == 'K') && lazer_on_off == false) || (NumMap[i + 1][j] == 'L' || NumMap[i][j] == 'L' || NumMap[i][j + 1] == 'L' || NumMap[i + 1][j + 1] == 'L')) {
                switch (WIN) {
                case 1: SetStartPosition(1120.0, 704.0); break;//возвращаем на начальные позиции
                case 2:SetStartPosition(160.0, 96.0); break;
                case 3:SetStartPosition(50.0, 50.0); break;
                }
                SetValues();
                time_to_win = 0;
                chekswamp = false;
            cout << "AAAAAAAAAA";
        }

        //------------------------------------ 

        if ((NumMap[i + 1][j] == '@' || NumMap[i][j] == '@' || NumMap[i][j + 1] == '@' || NumMap[i + 1][j + 1] == '@') && chekswamp == false) {
            Direction_X = Direction_X / 2;
            Direction_Y = Direction_Y / 2;
            chekswamp = true;
            cout << "BBBBBBBBBBBB";
        }

        //-----------------------------------

        if ((NumMap[i + 1][j] == 'z' || NumMap[i][j] == 'z' || NumMap[i][j + 1] == 'z' || NumMap[i + 1][j + 1] == 'z') && chekswamp == true) {
            Direction_X = Direction_X * 2;
            Direction_Y = Direction_Y * 2;
            chekswamp = false;
            cout << "CCCCCCCCCCCC";
        }
    }

    void Draw_Field(String* NumMap, int i, int j, int time_to_win, int WIN) { // выводит карту в зависимости от той, которую вы поставите в вызове метода 
            {
                if (NumMap[i][j] == ' ')  S_background.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ пробел, то рисуем 1й квадратик
                if (NumMap[i][j] == 's')  S_background.setTextureRect(IntRect(32, 0, 32, 32));//если встретили символ s, то рисуем 2й квадратик
                if ((NumMap[i][j] == '0')) S_background.setTextureRect(IntRect(64, 0, 32, 32));//если встретили символ 0, то рисуем 3й квадратик
                if ((NumMap[i][j] == 'w') && (time_to_win > time_to_complete_the_level[WIN-1]))  S_background.setTextureRect(IntRect(0, 0, 32, 32)); //если встретили символ w, то рисуем 4й квадратик
                if ((NumMap[i][j] == 'w') && (time_to_win < time_to_complete_the_level[WIN-1] || time_to_complete_the_level[WIN-1] == 0)) { S_background.setTextureRect(IntRect(96, 0, 32, 32));}
                if ((NumMap[i][j] == 'l')) S_background.setTextureRect(IntRect(128, 0, 32, 32));//если встретили символ l, то рисуем 5й квадратик
                if ((NumMap[i][j] == 'k') && lazer_on_off == true) S_background.setTextureRect(IntRect(160, 0, 32, 32));//если встретили символ k, то рисуем 6й квадратик
                if ((NumMap[i][j] == 'k') && lazer_on_off == false) S_background.setTextureRect(IntRect(0, 0, 32, 32));
                if ((NumMap[i][j] == '#')) S_background.setTextureRect(IntRect(192, 0, 32, 32));//если встретили символ #, то рисуем 7й квадратик
                if ((NumMap[i][j] == '@')) S_background.setTextureRect(IntRect(224, 0, 32, 32));//если встретили символ @, то рисуем 8й квадратик
                if ((NumMap[i][j] == 'L')) S_background.setTextureRect(IntRect(256, 0, 32, 32));//если встретили символ L, то рисуем 9й квадратик
                if ((NumMap[i][j] == 'K') && lazer_on_off == false) S_background.setTextureRect(IntRect(288, 0, 32, 32));//если встретили символ K, то рисуем 10й квадратик
                if ((NumMap[i][j] == 'K') && lazer_on_off == true) S_background.setTextureRect(IntRect(0, 0, 32, 32));
                if ((NumMap[i][j] == 'z')) S_background.setTextureRect(IntRect(320, 0, 32, 32));//если встретили символ z, то рисуем 11й квадратик

                S_background.setPosition(j * 32, i * 32);//по сути раскидывает квадратики, превращая в карту. то есть задает каждому из них позицию. если убрать, то вся карта нарисуется в одном квадрате 32*32 и мы увидим один квадрат

                window.draw(S_background);//рисуем квадратики на экран               

            }
    }

    void ResetBall() {
        switch (WIN)
        {
        case 1:SetStartPosition(1120.0, 704.0); break;
        case 2:SetStartPosition(160.0, 96.0); break;
        case 3:SetStartPosition(50.0, 50.0); break;
        }
        SetValues();
        time_to_win = 0;
    }

    void FieldClear() {
        window.clear();
    }

};

class Turn_on_music : public All_Sprite {
public:
    Music music;
    double sum_frame = 0;
    bool lazer_on_off = true;
    int  musicoff = 0;
    int chekmusic = 0;
    int NumMusic = 1;

    void Start_Music() { // Музыка (эфекты)
        music.openFromFile("jeremy-soule-sunrise-of-flutes.ogg");
        music.setVolume(50.f);
        music.play();
    }

    void On_Off_Music() {
        //включение и выключение звука
            if (musicoff == 0) { S_music_icon.setColor(Color::Red); musicoff = 1; chekmusic = 0; music.setVolume(0.f); }
            else if (musicoff == 1) { S_music_icon.setColor(Color::White); musicoff = 0; chekmusic = 0; music.setVolume(50.f); }
    }

    void NextMusic() { // убрать проверку на нажатие за цикл
        //смена музыки
         
            chekmusic = 0;
            switch (NumMusic) {
            case 1: music.openFromFile("Around_The_World.ogg"); music.play(); NumMusic++; break;
            case 2: music.openFromFile("Hyderabad.ogg"); music.play(); NumMusic++; break;
            case 3: music.openFromFile("music2.ogg"); music.play(); NumMusic++; break;
            case 4: music.openFromFile("marshmello.ogg"); music.play(); NumMusic++; break;


            default: NumMusic = 1;  music.openFromFile("jeremy-soule-sunrise-of-flutes.ogg"); music.play(); break;
            }
    }
};

class TEST : public Player_Ball{
public:
    void Test_Spawn() {
        assert(speed == 0);
        assert(Direction_X == 0 && Direction_Y == 0);
    }

    void Test_Flying_Ball(double SPEED) {
        assert(SPEED == 30.0);
    }
};


    //---------------------------------------------------------------------------------------------------------------------------------
int main()
{
    Playing_Field Ball;
    Playing_Field Map;
    Turn_on_music Song;
    TEST Test;

    double sum_frame = 0;
    int chekmusic=0;  //WIN - переключение сцен , sum - проверка на то, что эта сторона X во избежания багов (хотя они есть), chek_Y - если мячик летит больше 6 кадров сам, то он может опять удариться по оси Y
    float distance;
    //-------------------
    int PROVERKA = 0;
    //------------------------------------
    // Музыка (эфекты)    - Понять как правильно перекинуть в класс эту фигню
    Song.Start_Music();
    //-----------------------------------------------------------

    // Создаем спрайт и устанавливаем ему нашу текстуру


    Song.S_music_icon.setPosition(64, 0);
    Ball.S_next_button.setPosition(96, 0);
    Ball.S_reset.setPosition(128, 0);

    //--------------------------------РАБОТАЕТ
    Ball.SetStartPosition(1120.0, 704.0);
    Ball.S_ball.setPosition(Ball.position_X, Ball.position_Y);
    window.draw(Ball.S_ball);
    Test.Test_Spawn();
    //-----------------------------------
    while (window.isOpen())
    {

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //включает и выключает лазер
        sum_frame++;
        if (sum_frame == 500) {
            Ball.lazer_on_off = false; Map.lazer_on_off = false;
        }
        else if (sum_frame == 1000) { Ball.lazer_on_off = true; Map.lazer_on_off = true; sum_frame = 0; }

        //включение и выключение звука
        if (IntRect(64, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && Song.chekmusic == 100) {
            Song.On_Off_Music();
        }

        //смена музыки
        if (IntRect(96, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && Song.chekmusic == 100) {
            Song.NextMusic();
        }

        if (IntRect(128, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && Song.chekmusic == 100) {
            Song.chekmusic = 0;
            Ball.ResetBall();
        }

        if (Song.chekmusic != 100) Song.chekmusic++; // делаю задержку в 100 кадров для нажатия на кнопку выключения музыки, чтобы она не баговала 

        //проверка на нажатие лкм
        else  if (Mouse::isButtonPressed(Mouse::Left) && Ball.speed <= 0) {
            Ball.speed = 30.0;
            Ball.tempX = Mouse::getPosition(window).x;
            Ball.tempY = Mouse::getPosition(window).y;
            //----------------------
            distance = sqrt((Ball.tempX - Ball.position_X) * (Ball.tempX - Ball.position_X) + (Ball.tempY - Ball.position_Y) * (Ball.tempY - Ball.position_Y));//считаем дистанцию (длину от точки А до точки Б). формула длины вектора
            Ball.Direction_X += 0.1 * Ball.speed * (Ball.tempX - Ball.position_X) / distance;//идем по иксу с помощью вектора нормали
            Ball.Direction_Y += 0.1 * Ball.speed * (Ball.tempY - Ball.position_Y) / distance;//идем по игреку так же
            if (Ball.chekswamp == true) {
                Ball.Direction_X = Ball.Direction_X / 2;
                Ball.Direction_Y = Ball.Direction_Y / 2;
            }
        }


        if (Ball.speed > 0) {
            if (Ball.PROVERKA == 0) {
                Ball.position_X = Ball.position_X + Ball.Direction_X;
                Ball.position_Y = Ball.position_Y + Ball.Direction_Y;
                Test.Test_Flying_Ball(Ball.speed);
                Ball.time_to_win++;
            }
            //--------------------------------------------------
            if (Ball.WIN == 1) {
                for (int i = (int)Ball.position_Y / 32; i < ((int)Ball.position_Y + 32) / 32; i++) {//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
                    for (int j = (int)Ball.position_X / 32; j < ((int)Ball.position_X + 32) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
                    {
                        Ball.ContactObject_WithAnBall(TileMap, i, j);
                    }
                }
            }

            //-------------------------------------------------------------------------------------------------------------------------------------ПЕРЕХОД НА ВТОРУЮ КАРТУ
            if (Ball.WIN == 2) {
                for (int i = (int)Ball.position_Y / 32; i < ((int)Ball.position_Y + 32) / 32; i++) {//проходимся по тайликам, контактирующим с игроком,, то есть по всем квадратикам размера 32*32, которые мы окрашивали в 9 уроке. про условия читайте ниже.
                    for (int j = (int)Ball.position_X / 32; j < ((int)Ball.position_X + 32) / 32; j++)//икс делим на 32, тем самым получаем левый квадратик, с которым персонаж соприкасается. (он ведь больше размера 32*32, поэтому может одновременно стоять на нескольких квадратах). А j<(x + w) / 32 - условие ограничения координат по иксу. то есть координата самого правого квадрата, который соприкасается с персонажем. таким образом идем в цикле слева направо по иксу, проходя по от левого квадрата (соприкасающегося с героем), до правого квадрата (соприкасающегося с героем)
                    {
                        Ball.ContactObject_WithAnBall(TileMap_1, i, j);
                    }
                }
            }
            //----------------------------------------------------------------------------------------------------------------
            if (Ball.PROVERKA == 1) {

                cout << " hit in side Y | " << endl;
                //---------
                Ball.Direction_X = Ball.Direction_X;
                Ball.Direction_Y = -Ball.Direction_Y;
                Ball.PROVERKA = 0;
                //---------
                Ball.chek_X++;
            }

            //--------------------------------


            if (Ball.PROVERKA == 2) {
                cout << " hit in side X | chek_X = " << Ball.chek_X << "| ";

                //---------
                Ball.Direction_X = -Ball.Direction_X;
                Ball.Direction_Y = Ball.Direction_Y;//идем по игрику так же
                //---------

                if (Ball.chek_X >= 2) { Ball.Direction_Y = -Ball.Direction_Y; cout << "OOO"; }

                cout << endl;

                Ball.PROVERKA = 0;
            }


            Ball.chek_Y++; // Проверка по кадрам, для того, чтобы Y мог отталкнуться от Y1 (сделал для того, чтобы понимать, что мячик отскачил от стены , а не находится в ней)
            if (Ball.chek_Y == 3) {
                Ball.sum = 0;
                Ball.chek_Y = 0;
                Ball.chek_X = 0;
            }
            //--------------------------------------------------

        }
        //-------------------------------------  РАБОАТЕТ ->
        Map.FieldClear();


        if (Ball.WIN == 1)
            for (int i = 0; i < HEIGHT_MAP; i++)
                for (int j = 0; j < WIDTH_MAP; j++)
                {
                    Map.Draw_Field(TileMap, i, j, Ball.time_to_win, Ball.WIN);
                }
        if (Ball.WIN == 2)
            for (int i = 0; i < HEIGHT_MAP; i++)
                for (int j = 0; j < WIDTH_MAP; j++)
                {
                    Map.Draw_Field(TileMap_1, i, j, Ball.time_to_win, Ball.WIN);
                }

        //---------------------------------------------- ЧАСТЬ НЕ ОТРЕДАЧЕНА 


        Font font;//шрифт 
        font.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
        Text text1("", font, 18), text2("", font, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)
        ostringstream timeend, time;
        timeend << Ball.time_to_win;
        time << Ball.time_to_complete_the_level[Ball.WIN - 1];
        text1.setStyle(sf::Text::Bold | sf::Text::Underlined);//жирный и подчеркнутый текст. по умолчанию он "худой":)) и не подчеркнутый
        text2.setStyle(sf::Text::Bold | sf::Text::Underlined);
        if (Ball.WIN == 1){
            text1.setString("ЭТО ОБУЧАЮЩИЙ УРОВЕНЬ, ЧТОБЫ ПЕРЕЙТИ НА СЛЕДУЮЩИЙ, ПОПАДИ МЯЧОМ В СЕРЫЙ КВАДРАТ\n ПОДСКАЗКА : КЛИКНИ НА ЭКРАН, ЧТОБЫ ДАТЬ НАПРАВЛЕНИЕ");//задает строку тексту
            text1.setPosition(200, 0);
        }
        else {
            text1.setString("Таймер:" + timeend.str());//задает строку тексту
            text2.setString("Время на прохождение уровня:" + time.str());
            text1.setPosition(400, 0);
            text2.setPosition(700, 0);
        }
        
        //---------------------------------------------    ДО СЮДА


        Ball.S_ball.setPosition(Ball.position_X, Ball.position_Y);
        window.draw(Song.S_music_icon);
        window.draw(Ball.S_next_button);
        window.draw(Ball.S_reset);
        window.draw(text1);//рисую этот текст
        window.draw(text2);
        window.draw(Ball.S_ball);
        window.display();
    }
    return 0;
}