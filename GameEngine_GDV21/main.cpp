#include <glut.h>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

float moveX = 0.0f;
float moveY = -25.0f;

float moveX2 = 0.0f;
float moveY2 = 25.0f;

int frames = 0;
int currenttime;
int timebase = 0;
int previousTime = 0;
float deltaTime = 0.0f;
float randomVal = 1;
float randomYVal = 1;

bool keyStates[256] = { false };

int score1 = 0;
int score2 = 0;

bool isScored = false;

class GameObject
{
public:
    float posX, posY, posZ;
    float rotX, rotY, rotZ, angle;
    float scaleX, scaleY, scaleZ;
    float velX, velY, velZ;

    GameObject()
    {
        //Position
        posX = 0.0f;
        posY = 0.0f;
        posZ = 0.0f;
        //Rotation
        rotX = 0.0f;
        rotY = 0.0f;
        rotZ = 0.0f;
        //Scale
        scaleX = 1.0f;
        scaleY = 1.0f;
        scaleZ = 1.0f;
        //Velocity
        velX = 0.0f;
        velY = 0.0f;
        velZ = 0.0f;
    }

    void SetPosition(float posX, float posY, float posZ)
    {
        this->posX = posX;
        this->posY = posY;
        this->posZ = posZ;
    }

    void SetRotation(float rotX, float rotY, float rotZ, float angle)
    {
        this->rotX = rotX;
        this->rotY = rotY;
        this->rotZ = rotZ;
        this->angle = angle;
    }

    void SetScale(float scaleX, float scaleY, float scaleZ)
    {
        this->scaleX = scaleX;
        this->scaleY = scaleY;
        this->scaleZ = scaleZ;
    }

    void SetVelocity(float velX, float velY, float velZ)
    {
        int verticalityDeterminant = (rand() % 2);
        this->velX = velX;
        if (verticalityDeterminant == 1)
        {
            cout << "\nWe're going DOWN RIGHT NOW" << endl;
            this->velY = velY * -1;
        }
        else
        {
            cout << "\nWe're going UP UP UP" << endl;
            this->velY = velY;
        }
        this->velZ = velZ;
    }
};

class ObjectScene : public GameObject
{
public:

    void Draw()
    {
        //as much as possible we want our player to 1x1 scale
        glBegin(GL_QUADS);
        glVertex3f(posX - scaleX / 2, posY - scaleY / 2, 0.0f);
        glVertex3f(posX + scaleX / 2, posY - scaleY / 2, 0.0f);
        glVertex3f(posX + scaleX / 2, posY + scaleY / 2, 0.0f);
        glVertex3f(posX - scaleX / 2, posY + scaleY / 2, 0.0f);
        glEnd();
    }

    bool CheckCollision(GameObject o)
    {
        if (((posX + scaleX / 2) > (o.posX - o.scaleX / 2)) //(AR > BL)
            && ((posY - scaleY / 2) < (o.posY + o.scaleY / 2)) //(AB < BT)
            && ((posX - scaleX / 2) < (o.posX + o.scaleX / 2)) // (AL < BR)
            && ((posY + scaleY / 2) > (o.posY - o.scaleY / 2)) // (AT > BB)
            )
            return true;
        else
            return false;
    }

    void Velocity()
    {
        posX += velX * deltaTime;
        posY += velY * deltaTime;
        posZ += velZ * deltaTime;
    }
}ball_obj;

int RandomAngle_X()
{
    int randomRoll = rand() % 2;
    if (randomRoll == 1)
    {
        //cout << "\nI'm a POSITIVE ROLL" << endl;
        int toReturn = (rand() % 4) + 1;
        return toReturn * (rand() % 10) + 1;
    }

    else
    {
        //cout << "\nI'm a NEGATIVE ROLL" << endl;
        int toReturn = (rand() % 4) + 1;
        return (toReturn * -1) * (rand() % 10) + 1;
    }

}

int RandomVerticality()
{
    int randomYVelocity = (rand() % 20) + 10;
    return randomYVelocity;
}

void RenderScene(void)
{
    ObjectScene player_1, player_2, barrierLeft, barrierRight;
    srand(time(NULL));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // FPS Counter and Delta Time Calculation
    frames++;
    currenttime = glutGet(GLUT_ELAPSED_TIME);
    if (currenttime - timebase > 1000) {
        frames * 1000.0f / (currenttime - timebase);
        timebase = currenttime;
        frames = 0;
    }

    deltaTime = (currenttime - previousTime) / 1000.0f;
    previousTime = currenttime;

    //reset all matrices
    glLoadIdentity();
    //Set the Camera
    gluLookAt(0.0f, 0.0f, 100.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    //barrier
    barrierLeft.SetScale(2.0f, 100.0f, 0.0f);
    barrierLeft.SetPosition(-41.0f, 0.0f, 0.0f);
    barrierLeft.Draw();

    if (barrierLeft.CheckCollision(ball_obj) && ball_obj.velX < 0)
    {
        cout << "Collision Detection of Barrier Left!" << endl;
        ball_obj.velX = -ball_obj.velX;
        ball_obj.velX += (rand() % 31 - 10.0f);
        ball_obj.posX = barrierLeft.posX + barrierLeft.scaleX / 2 + barrierLeft.scaleX / 2;
    }
    barrierRight.SetScale(2.0f, 100.0f, 0.0f);
    barrierRight.SetPosition(41.0f, 0.0f, 0.0f);
    barrierRight.Draw();
    if (barrierRight.CheckCollision(ball_obj) && ball_obj.velX > 0)
    {
        cout << "Collision Detection of Barrier Right!" << endl;
        ball_obj.velX = -ball_obj.velX;
        ball_obj.velX += (rand() % 31 - 10.0f);
        ball_obj.posX = barrierRight.posX - barrierRight.scaleX / 2 - barrierRight.scaleX / 2;
    }
    // Player 1 (platform)
    player_1.SetScale(35.0f, 5.0f, 0.0f);
    player_1.SetPosition(moveX, moveY, 0.0f);
    player_1.Draw();
    if (player_1.CheckCollision(ball_obj) && ball_obj.velY < 0)
    {
        cout << "Collision Detection!" << endl;
        ball_obj.velY = -ball_obj.velY;
        ball_obj.velX += (rand() % 31 - 10.0f);
        ball_obj.posY = player_1.posY + player_1.scaleY / 2 + ball_obj.scaleY / 2;
    }

    if (player_1.CheckCollision(barrierLeft))
    {
        moveX = -22.0f;
    }

    else if (player_1.CheckCollision(barrierRight))
    {
        moveX = 22.0f;
    }


    // Player 2 (platform)
    player_2.SetScale(35.0f, 5.0f, 0.0f);
    player_2.SetPosition(moveX2, moveY2, 0.0f);
    player_2.Draw();
    if (player_2.CheckCollision(ball_obj) && ball_obj.velY > 0)
    {
        cout << "Collision Detection!" << endl;
        ball_obj.velY = -ball_obj.velY;
        ball_obj.velX += (rand() % 31 - 10.0f);
        ball_obj.posY = player_2.posY - player_2.scaleY / 2 - ball_obj.scaleY / 2;
    }

    //THIS IS TO PAUSE MOVEMENT WHEN BARRIER COLLIDING//
    if (player_2.CheckCollision(barrierLeft))
    {
        moveX2 = -22.0f;
    }

    else if (player_2.CheckCollision(barrierRight))
    {
        moveX2 = 22.0f;
    }

    //THIS IS TO SCORE COUNT//
    if ((ball_obj.posY > 54.0f && !isScored))
    {
        isScored = true;
        ++score1;
        cout << "\n=============================";
        cout << "\nTHE TOPSIDERZ SCORED!!!";
        cout << "\nSCOREBOARD: " << score1 << " - " << score2;
        cout << "\n=============================";
    }

    else if ((ball_obj.posY < -54.0f && !isScored))
    {
        isScored = true;
        ++score2;
        cout << "\n=============================";
        cout << "\nTHE BOTTOMFEEDERZ SCORED!!!";
        cout << "\nSCOREBOARD: " << score1 << " - " << score2;
        cout << "\n=============================";
    }

    // ball (moving square) - Update position based on velocity
    ball_obj.SetScale(5.0f, 5.0f, 5.0f);
    ball_obj.Velocity();
    if ((ball_obj.posY > 55.0f || ball_obj.posY < -55.0f))
    {
        isScored = false;
        ball_obj.SetPosition(0.0f, 17.5f, 0.0f);
        ball_obj.SetVelocity(randomVal, randomYVal, 0.0f);
    }
    ball_obj.Draw();


    //Input states
    if (keyStates['a'])
        moveX -= 1.0f;
    if (keyStates['d'])
        moveX += 1.0f;
    if (keyStates['j'])
        moveX2 -= 1.0f;
    if (keyStates['l'])
        moveX2 += 1.0f;
    if (keyStates['r'])
    {
        ball_obj.SetPosition(0.0f, 0.0f, 0.0f);
        randomVal = RandomAngle_X();
        randomYVal = RandomVerticality();
        ball_obj.SetVelocity(randomVal, randomYVal, 0.0f);

    }
    glutSwapBuffers();
}

void ChangeSize(int w, int h)
{
    //height or width should not be equal to 0
    if (h == 0)
        h = 1;
    float ratio = 1.0 * w / h;
    //fix the matrixmode to projection 
    glMatrixMode(GL_PROJECTION);
    //Reset the value of matrices
    glLoadIdentity();
    //Set the viewport of the window
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 1000);
    // Get Back to Model View
    glMatrixMode(GL_MODELVIEW);
}

void KeyDown(unsigned char key, int x, int y)
{
    keyStates[key] = true;
}

void KeyUP(unsigned char key, int x, int y)
{
    keyStates[key] = false;
}

void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(16, Timer, 0);
}

int main(int argc, char** argv)
{

    srand(time(NULL));
    randomVal = RandomAngle_X();
    randomYVal = RandomVerticality();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(320, 320);
    glutCreateWindow("EL PONG");
    //Ball 
    ball_obj.SetPosition(0.0f, 17.5f, 0.0f);
    ball_obj.SetVelocity(randomVal, randomYVal, 0.0f);

    //register callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    //glutIdleFunc(RenderScene);
    glutTimerFunc(16, Timer, 0);
    glutKeyboardFunc(KeyDown);
    glutKeyboardUpFunc(KeyUP);
    glEnable(GL_DEPTH_TEST);
    // enter GLUT event processing cycle
    glutMainLoop();
    return 1;
}