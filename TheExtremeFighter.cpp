#include <iostream>
#include<math.h>
#include<list>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;
using namespace sf;
const int w = 1200;
const int h = 800;
float dtr = 0.017453f;
class Animation
{
    public:
    float frm, speed;
    Sprite sprite;
    vector <IntRect> frames;
    Animation(){}
    Animation(Texture &t, int x, int y, int w1, int h1, int cnt, float Speed)
    {
        frm = 0;
        speed = Speed;
        for(int i = 0; i < cnt; i++)
        frames.push_back(IntRect(x+i*w1, y, w1, h1));
        sprite.setTexture(t);
        sprite.setOrigin(w1 / 2, h1 / 2);
        sprite.setTextureRect(frames[0]);
    }

    void update()
    {
        frm += speed;
        int n = frames.size();
        if(frm >= n) frm =- n;
        if(n > 0) sprite.setTextureRect(frames[int(frm)]);
    }
};



class Entity
{
public:
    float x,y,dx,dy,R,angle;
    bool life;
    string name;
    Animation anim;
    Entity()
    {
        life=1;
    }
    void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)
    {
        x=X;
        y=Y;
        anim=a;
        angle=Angle;
        R=radius;
    }
     virtual void update(){};
    void draw(RenderWindow &window)
    {
        anim.sprite.setPosition(x,y);
        anim.sprite.setRotation(angle+90);
        window.draw(anim.sprite);

    }
};



class asteroid : public Entity
{
public:
    asteroid()
    {
        dx=rand()%8-4;
        dy=rand()%8-4;
        name="asteroid";
    }
    void update()
    {
        x+=dx;
        y+=dy;
        if(x>w)x=0;if(x<0)x=w;
        if(y>h)y=0;if(y<0) y=h;

    }
};



class bullet : public Entity
{
    public:
    bullet()
    {
        name = "bullet";
    }
    void update()
    {
        dx = cos(angle*dtr)*6;
        dy = sin(angle*dtr)*6;
        x += dx;
        y += dy;
        if(x > w || x < 0 || y > h || y < 0) life = 0;
    }
};


bool isCollide(Entity *a, Entity *b)
{
    return (b->x - a->x)*(b->x - a->x)+
    (b->y - a->y)*(b->y - a->y)<
    (a->R + b->R)*(a->R + b->R);
}



int main()
{


    RenderWindow window(VideoMode(w, h) , "THE EXTREME FIGHTER");
    window.setFramerateLimit(60);

    Texture t1, t2, te, tm, texp, t4, t5;
    window.setFramerateLimit(60);

    t1.loadFromFile("spaceship.png");
    t2.loadFromFile("nassa.jpg");
    te.loadFromFile("station.jpg");
    tm.loadFromFile("moon.jpg");
    //texp.loadFromFile("expa.png");
    t4.loadFromFile("rock.png");
    t5.loadFromFile("fire_blue.png");

    Animation sBullet(t5, 0, 0, 32, 64, 16, 0.8);

    Sprite s1(t1), s2(t2), se(te), sm(tm), sexp(texp);
    s1.setTextureRect(IntRect(40, 0, 40, 40));
    s1.setOrigin(20, 20);

    Animation sRock(t4, 0, 0, 64, 64, 16, 0.2);
    sRock.sprite.setPosition(400, 400);
    se.setPosition(0, 620);
        sm.setPosition(600, 300);

    list<Entity*> entities;
    for(int i = 0; i < 15; i++)
    {
        asteroid *a = new asteroid();
        a->settings(sRock, rand()%w, rand()%h, rand()%360, 25);
        entities.push_back(a);
    }

    sexp.setPosition(300, 300);

    float frm = 0;
    float expspeed = 0.5;
    int frmcnt = 20;

    float x = 300, y =300;
    float dx = 0, dy = 0, angle = 0;
    bool thrust;

    //Clock clck;
    //float time;


    /*Music music;
    if(!music.loadFromFile("shd.ogg"))
        cout << " Failed!" << endl;
    music.play();*/


    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
                if(event.type == Event::KeyPressed)
                    if(event.key.code == Keyboard::Space)
                {
                    bullet *b = new bullet();
                    b->settings(sBullet, x, y, angle, 10);
                    entities.push_back(b);
                }
        }


        frm += expspeed;
        if(frm > frmcnt) frm -= frmcnt;
        sexp.setTextureRect(IntRect(int(frm)*50, 0, 50, 50));

        if(Keyboard::isKeyPressed(Keyboard::Right)) angle += 3;
        if(Keyboard::isKeyPressed(Keyboard::Left)) angle -= 3;
        if(Keyboard::isKeyPressed(Keyboard::Up)) thrust = true;
         else thrust = false;

         for(auto a:entities)
           for(auto b:entities)
           if(a->name=="asteroid" && b->name =="bullet")
           if(isCollide(a, b))
         {
             a->life = false; b->life = false;
         }


        if(thrust)
        {
            dx += cos(angle*dtr) * 0.2;
            dy += sin(angle*dtr) * 0.2;
        }
        else
        {
            dx *= 0.2;
            dy *= 0.2;
        }

        int maxspeed = 15;
        float speed =(dx *dx + dy * dy);
        if(speed > maxspeed)
        {
            dx *= maxspeed / speed;
            dy *=  maxspeed / speed;
        }

        x += dx;
        y += dy;

        if(x > w) x = 0;
        if(x < 0) x = w;
        if(y > h) y = 0;
        if(y < 0) y = h;

        s1.setPosition(x, y);
        s1.setRotation(angle + 90);


        for(auto i = entities.begin(); i!= entities.end(); )
        {
            Entity *e = *i;
            e->update();
            e->anim.update();

            if(e->life == false)
            {
                i = entities.erase(i); delete e;
            }
            else i++;
        }





        sRock.update();

        window.clear();
        //window.draw(sRock.sprite);
        //sRock.update();
        window.draw(s2);
        window.draw(se);
        window.draw(sm);


        window.draw(s1);


        //sRock.update();
        //window.draw(sRock.sprite);
        //window.draw(sexp);
        for(auto i:entities) i->draw(window);
        window.display();
    }
    return 0;
}




