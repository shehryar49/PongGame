#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
enum game_state
{
  IN_GAME,
  IN_MENU1,
  IN_MENU2,
  SHOW_SCORES,
  OVER //game over await mouse click to go again

};
string names[5];
int scores[5];
void sortScores()
{
  for(int i=1;i<=4;i++)
  {
    for(int j=0;j<5-i;j++)
    {
      if(scores[j] < scores[j+1])
      {
        int c = scores[j];
        scores[j] = scores[j+1];
        scores[j+1] = c;
        string s = names[j];
        names[j] = names[j+1];
        names[j+1] = s;
      }
    }
  }
}
bool readScores()
{
  ifstream fin("scores.txt");
  if(!fin)
    return false;
  for(int i=0;i<5;i++)
  {
    fin>>scores[i];
    fin.ignore();
    std::getline(fin,names[i]);
  }
  fin.close();
  sortScores();
  return true;
}
void writeScores()
{
  ofstream out("scores.txt",ios::out);
  for(int i=0;i<5;i++)
  {
    out<<scores[i]<<" "<<names[i]<<endl;
  }
  out.close();
}
class Button
{
private:
  sf::RectangleShape rect;
  sf::Font font;
  sf::Text text;
public:
  Button()
  {
     font.loadFromFile("semicoder.otf");
     text.setFont(font);
  }
  void setDimensions(float width,float height)
  {
    rect.setSize(sf::Vector2f(width,height));
  }
  void setText(string s,int chSize=32,sf::Color color=sf::Color::Black)
  {
    text.setString(s);
    text.setCharacterSize(chSize);
    rect.setFillColor(color);
  }
  void setOutline(sf::Color color = sf::Color::White,int thickness =1)
  {
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(1);
  }
  void setPosition(sf::Vector2f pos)
  {
    rect.setPosition(pos);
    text.setPosition(sf::Vector2f(pos.x+10,pos.y+2));
  }
  sf::Vector2f getPosition()
  {
     return rect.getPosition();
  }
  void render(sf::RenderWindow& w)
  {
    w.draw(rect);
    w.draw(text);
  }
  void setBgColor(sf::Color color)
  {
    rect.setFillColor(color);
  }
  void setColor(sf::Color color)
  {
    text.setFillColor(color);
  }
};
class PongMenu
{
private:
  Button* buttons;
  int n;
  sf::Texture t;
  sf::Sprite sprite;
public:
  PongMenu()
  {
    buttons = nullptr;
  }
  void Init(string* options,int N,int chSize=32,int hideBtnBg=false,float x=350)
  {
    if(buttons)
      delete[] buttons;
    n = N;
    t.loadFromFile("bg2.jpg");
    sprite.setTexture(t);
    buttons = new Button[N];
    int y = 200;
    for(int i=0;i<N;i++)
    {
      buttons[i].setText(options[i],chSize);
      if(hideBtnBg)
        buttons[i].setDimensions(0,0);
      else
        buttons[i].setDimensions(110,50);
      buttons[i].setPosition(sf::Vector2f(x,y));
      buttons[i].setOutline();
      y+=80;
    }
  }
  void render(sf::RenderWindow& win)
  {
     win.clear();
     win.draw(sprite);
     for(int i=0;i<n;i++)
       buttons[i].render(win);
  }
  void processMouseMove(sf::Vector2i pos)
  {
     for(int i=0;i<n;i++)
     {
       sf::Vector2f btnPos = buttons[i].getPosition();
       
       if(pos.x >= btnPos.x && pos.y>=btnPos.y && pos.x<btnPos.x+110 && pos.y<btnPos.y+50)
       {
        buttons[i].setBgColor(sf::Color::White);
        buttons[i].setColor(sf::Color::Black);
       }
       else
       {
        buttons[i].setBgColor(sf::Color::Black);
        buttons[i].setColor(sf::Color::White);
       }
     }
  }
  int processMouseClick(sf::Vector2i pos)
  {
     for(int i=0;i<n;i++)
     {
       sf::Vector2f btnPos = buttons[i].getPosition();
       
       if(pos.x >= btnPos.x && pos.y>=btnPos.y && pos.x<btnPos.x+110 && pos.y<btnPos.y+50)
       {
        //buttons[i] is clicked
        return i; //ith button is clicked
       }
     }
     return -1;
  }
  ~PongMenu()
  {
    if(buttons)
      delete[] buttons;
  }
};
class Bat
{
private:
  sf::RectangleShape rect;
public:
  void setSize(float x,float y)
  {
    rect.setSize(sf::Vector2f(x,y));
  }
  void setFillColor(sf::Color color = sf::Color::White)
  {
    rect.setFillColor(color);
  }
  void setPosition(float x,float y)
  {
    rect.setPosition(sf::Vector2f(x,y));
  }
  auto x()
  {
    return rect.getPosition().x;
  }
  auto y()
  {
    return rect.getPosition().y;
  }
  void render(sf::RenderWindow& win)
  {
    win.draw(rect);
  }
  auto getGlobalBounds()
  {
    return rect.getGlobalBounds();
  }
};
class Ball
{
private:
  sf::RectangleShape rect;
  float velX;
  float velY;
public:
  void setSize(float x,float y)
  {
    rect.setSize(sf::Vector2f(x,y));
  }
  void setFillColor(sf::Color color = sf::Color::White)
  {
    rect.setFillColor(color);
  }
  void setPosition(float x,float y)
  {
    rect.setPosition(sf::Vector2f(x,y));
  }
  auto x()
  {
    return rect.getPosition().x;
  }
  auto y()
  {
    return rect.getPosition().y;
  }
  void render(sf::RenderWindow& win)
  {
    win.draw(rect);
  }
  auto getGlobalBounds()
  {
    return rect.getGlobalBounds();
  }
  auto vX()const
  {
    return velX;
  }
  auto vY()const
  {
    return velY;
  }
  void setVelocity(float vX,float vY)
  {
    velX = vX;
    velY = vY;
  }
  void invertVX()
  {
    velX*=-1;
  }
  void invertVY()
  {
    velY*=-1;
  }
  
};

class PongGame
{
private:
  sf::RenderWindow window;
  sf::VertexArray lines;
  sf::Font f;
  sf::Text daScore;
  sf::Text winnerText;
  sf::Music m;
  Bat bat1;
  Bat bat2;
  Ball ball;
  PongMenu menu;
  game_state gameState;
  int score1 = 0;
  int score2 = 0;
  bool vsComputer = true;
  int difficulty = 1;
  int maxScore;
  bool hit = true;
  int counter =0;
public:
  bool isRunning;
  PongGame(int maxS) : window(sf::VideoMode(800, 600), "Pong Game",sf::Style::Titlebar | sf::Style::Close)
  {
    readScores();
    maxScore = maxS;
    gameState = IN_MENU1;
    bat1.setSize(15,150);
    bat2.setSize(15,150);
    bat1.setPosition(5,225);
    bat2.setPosition(780,225); 
    lines.setPrimitiveType(sf::LinesStrip);
    lines.resize(2);  
    lines[0].position = sf::Vector2f(400, 0);
    lines[1].position = sf::Vector2f(400, 600);
    ball.setSize(15,15);
    ball.setFillColor(sf::Color::Green);
    ball.setPosition(385,300);
    ball.setVelocity(-250,-250);
    window.setFramerateLimit(60);
    f.loadFromFile("SUBWT___.ttf");
    daScore.setFont(f);
    daScore.setString("0  0");
    daScore.setCharacterSize(32);
    daScore.setPosition(370,0);
    winnerText.setFont(f);
    winnerText.setPosition(325,280);
    if(!m.openFromFile("champion.wav"))
      cout<<"Error opening file!"<<endl;
    m.setLoop(true);
    m.play();
    isRunning = true;
    string options[]={"New","Score","Exit"};
    menu.Init(options,3);
    counter = 0;
  }
  void resetGame()
  {
    bat1.setPosition(5,225);
    bat2.setPosition(780,225);
    score1 = 0;
    score2 = 0;
    ball.setPosition(385,300);
    daScore.setFont(f);
    daScore.setString("0  0");
    hit = true;
    counter = 0;
  }
  void pollEvents()
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            isRunning = false;
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
          if(event.mouseButton.button == sf::Mouse::Left)
          {
            int i = menu.processMouseClick(sf::Mouse::getPosition(window));
            if(gameState==IN_MENU1)
            {
              if(i==0)//new game
              {
                gameState = IN_MENU2;
                string options[]={"Easy","Medium","Deadly","2 Players"};
                menu.Init(options,4,17);
              }
              else if(i==1)//scores
              {
                if(readScores())
                {
                  gameState = SHOW_SCORES;
                  string* options = new string[5];
                  for(int i=0;i<5;i++)
                    options[i] = names[i]+"  "+to_string(scores[i]);
                  menu.Init(options,5,15,true,320);
                  delete[] options;
                }
              }
              else if(i==2)
            {
              isRunning = false;
            }
            }
            else if(gameState==IN_MENU2)
            {
              if(i==0)
              {
                gameState = IN_GAME;
                difficulty = 1;
                m.openFromFile("mr_robot_theme.wav");
                m.play();
              }
              else if(i==1)
              {
                gameState = IN_GAME;
                m.openFromFile("mr_robot_theme.wav");
                m.play();
                difficulty = 2;
              }
              else if(i==2)
              {
                gameState = IN_GAME;
                difficulty = 3;
                m.openFromFile("mr_robot_theme.wav");
                m.play();
              }
              else if(i==3)
              {
                gameState = IN_GAME;
                difficulty = -1; //indicates 2 player game
                vsComputer = false;
                m.openFromFile("mr_robot_theme.wav");
                m.play();
              }
              
            }
            else if(gameState==SHOW_SCORES)
            {
              gameState = IN_MENU1;
              string options[]={"New","Score","Exit"};
              menu.Init(options,3);
            }
            else if(gameState==OVER)
            {
              resetGame();
              gameState = IN_GAME;
            }
          }
        }
        else if(event.type == sf::Event::MouseMoved && gameState!=IN_GAME && gameState!=SHOW_SCORES)
        {
          menu.processMouseMove(sf::Mouse::getPosition(window));
        }
        else if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::W && bat1.y()>=20)
            {
            bat1.setPosition(bat1.x(),bat1.y()-20);
            }
            else if(event.key.code == sf::Keyboard::S && bat1.y()<=450)
            {
            bat1.setPosition(bat1.x(),bat1.y()+20);
            }
            else if(!vsComputer && event.key.code == sf::Keyboard::Up && bat2.y()>=20)
            {
            bat2.setPosition(bat2.x(),bat2.y()-20);
            }
            else if(!vsComputer && event.key.code == sf::Keyboard::Down && bat2.y()<=450)
            {
            bat2.setPosition(bat2.x(),bat2.y()+20);
            }
            
        }
    }
  }
  void update(float dt)
  {
    pollEvents();
    bool hitByComputer = false;
    bool hitByP1 = false;
    bool collision = false;
    if(gameState != IN_GAME)
      return;
      //if it's against computer then update bat2's position as well
      if(vsComputer && hit)
      {
          if(ball.y()<465)
          {
            bat2.setPosition(bat2.x(),ball.y()-15);
          }
      }
      //detect collisions
      //advanced mathematics
      //sorry kiddos
      if(bat1.getGlobalBounds().intersects(ball.getGlobalBounds()) )
      {
          ball.invertVX();
          hitByP1 = true;
          collision = true;
      }
      else if(bat2.getGlobalBounds().intersects(ball.getGlobalBounds()) )
      {
          ball.invertVX();
          collision = true;
          hitByComputer = vsComputer;
      }
      if(hitByComputer && difficulty==1)
        hit = !hit;
      else if(hitByComputer && difficulty==2)
      {
          counter+=1;
          if(counter%2==0)// miss next shot 
            hit = false;
          else
            hit=true;
      }
      
      ball.setPosition(ball.x() + ball.vX()*dt,ball.y() + ball.vY()*dt);
      if(ball.x()<=-15)//missed by player 1
      {
          score2+=1;
          ball.setPosition(385,300);
          stringstream ss;
          ss<<score1<<"  "<<score2;
          daScore.setString(ss.str());
          if(score2==maxScore)
          {
            gameState = OVER;
            if(score2 > score1)
            {
              if(vsComputer)
                winnerText.setString("You lose!");
              else
                  winnerText.setString("Player 2 won!");
            }
            else 
              winnerText.setString("Game Draw");
          }
      }
      else if(ball.x()>=800)//missed by bat2
      {
          ball.invertVX();
          score1+=1;
          ball.setPosition(385,300); 
          stringstream ss;
          ss<<score1<<"  "<<score2;
          daScore.setString(ss.str());
          if(vsComputer && (difficulty==1 || difficulty==2))
            hit = !hit;
          if(score1==maxScore)
          {
            gameState = OVER;
            if(score1>score2)
            {
              if(vsComputer)
              {
                winnerText.setString("You won!");
                  for(int i=0;i<5;i++)
                  {
                    if(scores[i] < score1)
                    {
                      render();
                      scores[i] = score1; //overwrite
                      cout<<"New High Score"<<endl;
                      cout<<"Enter name: ";
                      string str;
                      cin>>str;
                      names[i] = str;
                      writeScores();
                      break;
                    }
                }
              }
              else
                winnerText.setString("Player 1 won!");
            }
            else
              winnerText.setString("Game Draw");
          }
      }
      else if(ball.y()<=0 || ball.y()>=585)
         ball.invertVY();
  }
  void render()
  {
    window.clear(sf::Color::Black);
    if(gameState == IN_GAME)
    {
      //render bat objects on game's window
      bat1.render(window);
      bat2.render(window);
      window.draw(lines);
      ball.render(window);
      window.draw(daScore);
    }
    else if(gameState==OVER)
    {
      window.draw(winnerText);
    }
    else
      menu.render(window);
    window.display();
  }
  ~PongGame()
  {
    window.close();
  }
};

int main()
{
    sf::Clock dt_clock;
    float dt;
    PongGame game(5);
    while (game.isRunning)
    {
      dt = dt_clock.restart().asSeconds(); 
      //Update
      game.update(dt);
      //Render
      game.render();    
    }
    return 0;
}
