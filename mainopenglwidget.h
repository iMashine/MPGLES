#ifndef MAINOPENGLWIDGET_H
#define MAINOPENGLWIDGET_H

#include "emitter.h"
#include "emitterlist.h"
#include "shaderprogram.h"

#include <QBasicTimer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QOpenGLPaintDevice>

#include <QDebug>

class MainOpenGLWidget : protected QOpenGLWidget, protected QOpenGLFunctions
{
public:
    MainOpenGLWidget(QObject *parent = Q_NULLPTR);
    ~MainOpenGLWidget();

    EmitterList *GetEmitters();

    void Refresh();

    void ClearBuffers();

    void Draw(int starting_index, int indexes_count, int max_vertices);

    int GetWidth()
    {
        return width();
    }

    int GetHeight()
    {
        return height();
    }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void timerEvent(QTimerEvent *e) override;
    void initializeMP();

private:
    QBasicTimer timer;
    QMatrix4x4 m_projection;

    MP_Device *m_device;
    MP_Manager *m_manager;

    ShaderProgram m_program;
    QOpenGLFunctions m_functions;
    QObject *m_parent = Q_NULLPTR;

    QOpenGLPaintDevice *m_mainGLPaintDevice = nullptr;
    QPainter *m_painter = nullptr;

    TextureList m_textures;


//    QList<QOpenGLTexture *> m_textures;

    EmitterList *m_emitters;


public:
    float *m_vertices;
    unsigned char *m_colors;
    float *m_textureCoords;
    unsigned short *m_indexes;
};

#endif // MAINOPENGLWIDGET_H
