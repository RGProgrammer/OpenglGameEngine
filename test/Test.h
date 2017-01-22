#ifndef _TTB_TEST_SAMPLE_H_
#define _TTB_TEST_SAMPLE_H_
#include "../Core/Graphics/Window.h"
#include "../Core/Graphics/GLRenderer.h"
#include "../Core/Graphics/Model3D.h"
#include "../Core/Scene/GameScene.h"


namespace TTB {
    class Test {
    public :
        Test();
        ~Test();
        void Destroy();
        void Start();
        int Init();
    private:
        GLRenderer*         m_Renderer;
        GameScene*          m_CurrentScene ;
        PerspCamera*        m_Camera ;
    };
};
#endif // _TTB_TEST_SAMPLE_H_
