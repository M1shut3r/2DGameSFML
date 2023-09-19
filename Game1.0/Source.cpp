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
    int WIN = 1; //���������� ��� ������� ��������� ����, ����� ��� �������� � ���� �������� ������� (������� ������)
    int chek_X = 0;
    int sum = 0;
    double speed=0.0;
    double position_X, position_Y;
    double Direction_X=0, Direction_Y=0;

    void SetStartPosition(double position_X, double position_Y) { //����� ,������� ����� ��������� ���������� ���� �� ����(����� ��� ����, ��� � ��� ����� ������ � ����� ���� ����������)
        this->position_X = position_X;
        this->position_Y = position_Y;
    }

    void SetValues() { //���� ������� ��������, �� ��� �������� ����� 0
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
        // �������� ����� ����������� ��� ����� ��������
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

// ����� �������, ��� ������� ���� �������� ����������� �������� ����
class Playing_Field : public Player_Ball, public HIT , public All_Sprite {
public:
    int chek_Y = 0, time_to_complete_the_level[3] = {0 , 1250 , 500}, time_to_win=0;
    bool lazer_on_off = true, chekswamp = false;

    void ContactObject_WithAnBall(String* NumMap, int i, int j) { //�������� ���� �� ��������������� �� ���� ��������� �� ����� � ������� ������ ��������

        if ((NumMap[i + 1][j+1] == 's' || NumMap[i+1][j] == 's' || NumMap[i][j] == 's' ||  NumMap[i][j + 1] == 's' || NumMap[i + 1][j + 1] == '0' || NumMap[i + 1][j] == '0' || NumMap[i][j + 1] == '0' || NumMap[i][j] == '0')) { //���� ������ ����� 's' (������) ��� '0' (�����)
             

                position_X += Direction_X;//���� �� ���� � ������� ������� �������
                position_Y += -Direction_Y;//���� �� ������ ��� ��

                
                if (position_Y>=i*32 + 32 || position_Y<=i*32) { // test -----
                    if (position_X <=32){
                        position_X += -Direction_X * 2;//���� �� ���� � ������� ������� �������
                        position_Y += Direction_Y * 2;//���� �� ������ ��� ��
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
                    position_X += -Direction_X*2;//���� �� ���� � ������� ������� �������
                    position_Y += Direction_Y*2;//���� �� ������ ��� ��
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
            if(time_to_win < time_to_complete_the_level[WIN-1] || time_to_complete_the_level[WIN-1] == 0) // ������ ������� ��� ������� �� �����, ������ ��� ������������� �������, ����� ������������ ���� ������
            WIN++; //������ ����� �� ������
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
                case 1: SetStartPosition(1120.0, 704.0); break;//���������� �� ��������� �������
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

    void Draw_Field(String* NumMap, int i, int j, int time_to_win, int WIN) { // ������� ����� � ����������� �� ���, ������� �� ��������� � ������ ������ 
            {
                if (NumMap[i][j] == ' ')  S_background.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� ������ ������, �� ������ 1� ���������
                if (NumMap[i][j] == 's')  S_background.setTextureRect(IntRect(32, 0, 32, 32));//���� ��������� ������ s, �� ������ 2� ���������
                if ((NumMap[i][j] == '0')) S_background.setTextureRect(IntRect(64, 0, 32, 32));//���� ��������� ������ 0, �� ������ 3� ���������
                if ((NumMap[i][j] == 'w') && (time_to_win > time_to_complete_the_level[WIN-1]))  S_background.setTextureRect(IntRect(0, 0, 32, 32)); //���� ��������� ������ w, �� ������ 4� ���������
                if ((NumMap[i][j] == 'w') && (time_to_win < time_to_complete_the_level[WIN-1] || time_to_complete_the_level[WIN-1] == 0)) { S_background.setTextureRect(IntRect(96, 0, 32, 32));}
                if ((NumMap[i][j] == 'l')) S_background.setTextureRect(IntRect(128, 0, 32, 32));//���� ��������� ������ l, �� ������ 5� ���������
                if ((NumMap[i][j] == 'k') && lazer_on_off == true) S_background.setTextureRect(IntRect(160, 0, 32, 32));//���� ��������� ������ k, �� ������ 6� ���������
                if ((NumMap[i][j] == 'k') && lazer_on_off == false) S_background.setTextureRect(IntRect(0, 0, 32, 32));
                if ((NumMap[i][j] == '#')) S_background.setTextureRect(IntRect(192, 0, 32, 32));//���� ��������� ������ #, �� ������ 7� ���������
                if ((NumMap[i][j] == '@')) S_background.setTextureRect(IntRect(224, 0, 32, 32));//���� ��������� ������ @, �� ������ 8� ���������
                if ((NumMap[i][j] == 'L')) S_background.setTextureRect(IntRect(256, 0, 32, 32));//���� ��������� ������ L, �� ������ 9� ���������
                if ((NumMap[i][j] == 'K') && lazer_on_off == false) S_background.setTextureRect(IntRect(288, 0, 32, 32));//���� ��������� ������ K, �� ������ 10� ���������
                if ((NumMap[i][j] == 'K') && lazer_on_off == true) S_background.setTextureRect(IntRect(0, 0, 32, 32));
                if ((NumMap[i][j] == 'z')) S_background.setTextureRect(IntRect(320, 0, 32, 32));//���� ��������� ������ z, �� ������ 11� ���������

                S_background.setPosition(j * 32, i * 32);//�� ���� ����������� ����������, ��������� � �����. �� ���� ������ ������� �� ��� �������. ���� ������, �� ��� ����� ���������� � ����� �������� 32*32 � �� ������ ���� �������

                window.draw(S_background);//������ ���������� �� �����               

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

    void Start_Music() { // ������ (������)
        music.openFromFile("jeremy-soule-sunrise-of-flutes.ogg");
        music.setVolume(50.f);
        music.play();
    }

    void On_Off_Music() {
        //��������� � ���������� �����
            if (musicoff == 0) { S_music_icon.setColor(Color::Red); musicoff = 1; chekmusic = 0; music.setVolume(0.f); }
            else if (musicoff == 1) { S_music_icon.setColor(Color::White); musicoff = 0; chekmusic = 0; music.setVolume(50.f); }
    }

    void NextMusic() { // ������ �������� �� ������� �� ����
        //����� ������
         
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
    int chekmusic=0;  //WIN - ������������ ���� , sum - �������� �� ��, ��� ��� ������� X �� ��������� ����� (���� ��� ����), chek_Y - ���� ����� ����� ������ 6 ������ ���, �� �� ����� ����� ��������� �� ��� Y
    float distance;
    //-------------------
    int PROVERKA = 0;
    //------------------------------------
    // ������ (������)    - ������ ��� ��������� ���������� � ����� ��� �����
    Song.Start_Music();
    //-----------------------------------------------------------

    // ������� ������ � ������������� ��� ���� ��������


    Song.S_music_icon.setPosition(64, 0);
    Ball.S_next_button.setPosition(96, 0);
    Ball.S_reset.setPosition(128, 0);

    //--------------------------------��������
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

        //�������� � ��������� �����
        sum_frame++;
        if (sum_frame == 500) {
            Ball.lazer_on_off = false; Map.lazer_on_off = false;
        }
        else if (sum_frame == 1000) { Ball.lazer_on_off = true; Map.lazer_on_off = true; sum_frame = 0; }

        //��������� � ���������� �����
        if (IntRect(64, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && Song.chekmusic == 100) {
            Song.On_Off_Music();
        }

        //����� ������
        if (IntRect(96, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && Song.chekmusic == 100) {
            Song.NextMusic();
        }

        if (IntRect(128, 0, 32, 32).contains(Mouse::getPosition(window)) && Mouse::isButtonPressed(Mouse::Left) && Song.chekmusic == 100) {
            Song.chekmusic = 0;
            Ball.ResetBall();
        }

        if (Song.chekmusic != 100) Song.chekmusic++; // ����� �������� � 100 ������ ��� ������� �� ������ ���������� ������, ����� ��� �� �������� 

        //�������� �� ������� ���
        else  if (Mouse::isButtonPressed(Mouse::Left) && Ball.speed <= 0) {
            Ball.speed = 30.0;
            Ball.tempX = Mouse::getPosition(window).x;
            Ball.tempY = Mouse::getPosition(window).y;
            //----------------------
            distance = sqrt((Ball.tempX - Ball.position_X) * (Ball.tempX - Ball.position_X) + (Ball.tempY - Ball.position_Y) * (Ball.tempY - Ball.position_Y));//������� ��������� (����� �� ����� � �� ����� �). ������� ����� �������
            Ball.Direction_X += 0.1 * Ball.speed * (Ball.tempX - Ball.position_X) / distance;//���� �� ���� � ������� ������� �������
            Ball.Direction_Y += 0.1 * Ball.speed * (Ball.tempY - Ball.position_Y) / distance;//���� �� ������ ��� ��
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
                for (int i = (int)Ball.position_Y / 32; i < ((int)Ball.position_Y + 32) / 32; i++) {//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
                    for (int j = (int)Ball.position_X / 32; j < ((int)Ball.position_X + 32) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
                    {
                        Ball.ContactObject_WithAnBall(TileMap, i, j);
                    }
                }
            }

            //-------------------------------------------------------------------------------------------------------------------------------------������� �� ������ �����
            if (Ball.WIN == 2) {
                for (int i = (int)Ball.position_Y / 32; i < ((int)Ball.position_Y + 32) / 32; i++) {//���������� �� ��������, �������������� � �������,, �� ���� �� ���� ����������� ������� 32*32, ������� �� ���������� � 9 �����. ��� ������� ������� ����.
                    for (int j = (int)Ball.position_X / 32; j < ((int)Ball.position_X + 32) / 32; j++)//��� ����� �� 32, ��� ����� �������� ����� ���������, � ������� �������� �������������. (�� ���� ������ ������� 32*32, ������� ����� ������������ ������ �� ���������� ���������). � j<(x + w) / 32 - ������� ����������� ��������� �� ����. �� ���� ���������� ������ ������� ��������, ������� ������������� � ����������. ����� ������� ���� � ����� ����� ������� �� ����, ������� �� �� ������ �������� (���������������� � ������), �� ������� �������� (���������������� � ������)
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
                Ball.Direction_Y = Ball.Direction_Y;//���� �� ������ ��� ��
                //---------

                if (Ball.chek_X >= 2) { Ball.Direction_Y = -Ball.Direction_Y; cout << "OOO"; }

                cout << endl;

                Ball.PROVERKA = 0;
            }


            Ball.chek_Y++; // �������� �� ������, ��� ����, ����� Y ��� ������������ �� Y1 (������ ��� ����, ����� ��������, ��� ����� �������� �� ����� , � �� ��������� � ���)
            if (Ball.chek_Y == 3) {
                Ball.sum = 0;
                Ball.chek_Y = 0;
                Ball.chek_X = 0;
            }
            //--------------------------------------------------

        }
        //-------------------------------------  �������� ->
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

        //---------------------------------------------- ����� �� ���������� 


        Font font;//����� 
        font.loadFromFile("CyrilicOld.ttf");//�������� ������ ������ ���� ������
        Text text1("", font, 18), text2("", font, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
        ostringstream timeend, time;
        timeend << Ball.time_to_win;
        time << Ball.time_to_complete_the_level[Ball.WIN - 1];
        text1.setStyle(sf::Text::Bold | sf::Text::Underlined);//������ � ������������ �����. �� ��������� �� "�����":)) � �� ������������
        text2.setStyle(sf::Text::Bold | sf::Text::Underlined);
        if (Ball.WIN == 1){
            text1.setString("��� ��������� �������, ����� ������� �� ���������, ������ ����� � ����� �������\n ��������� : ������ �� �����, ����� ���� �����������");//������ ������ ������
            text1.setPosition(200, 0);
        }
        else {
            text1.setString("������:" + timeend.str());//������ ������ ������
            text2.setString("����� �� ����������� ������:" + time.str());
            text1.setPosition(400, 0);
            text2.setPosition(700, 0);
        }
        
        //---------------------------------------------    �� ����


        Ball.S_ball.setPosition(Ball.position_X, Ball.position_Y);
        window.draw(Song.S_music_icon);
        window.draw(Ball.S_next_button);
        window.draw(Ball.S_reset);
        window.draw(text1);//����� ���� �����
        window.draw(text2);
        window.draw(Ball.S_ball);
        window.display();
    }
    return 0;
}