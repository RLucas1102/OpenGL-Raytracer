#ifndef TRANSMANAGER_H
#define TRANSMANAGER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

class TransManager
{
public:
    
    int cap;
    int front;
    int rear;
    int numTransforms;
    glm::mat4* manager;

    TransManager() 
    {
        front = 0;
        rear = -1;
        cap = 1000000;
        numTransforms = 0;
        manager = new glm::mat4[cap];
    }

    ~TransManager()
    {
        delete[] manager;
    }


    void AddTransform(glm::mat4 transMat) 
    {
        if (rear > cap) {
            RemoveTransform();
        }

        manager[++rear] = transMat;
        numTransforms++;
    }

    glm::mat4 GetTransform(int index)
    {
        return manager[index];
    }

    int GetSize()
    {
        return numTransforms;
    }

private:

    void RemoveTransform()
    {
        if (front > rear) {
            std::cout << "Manager empty" << std::endl;
            return;
        }

        for (int i = 0; i < rear; i++) {
            manager[i] = manager[i + 1];
        }

        rear--;
        numTransforms--;
    }

};
#endif
