#ifndef TTB_CAMERA_TYPE_H_
#define TTB_CAMERA_TYPE_H_
#include "..//Common//Math.h"
#include <stdlib.h>
namespace TTB{

	class Camera {
	public:
		Camera();
		virtual ~Camera();
		void RotateViaDirection (_float Angle);
		void RotateViaUp        (_float Angle);
		void RotateViaLeft      (_float Angle);
		void Translate          (Vertex3d ver);
		void setPosition(Vertex3d Pos);
		bool setOrientation(Vertex3d Dir,Vertex3d Up);
		Vertex3d getPosition();
		Vertex3d getDirection();
		Vertex3d getUp();
		Vertex3d getLeft();
		_float getNearValue();
		_float getFarValue();
		_float* getViewMtx();
		_float* getProjectionMtx();
	protected:
        Vertex3d			m_Pos;
        Vertex3d 			m_Dir ;
        Vertex3d			m_Up;
        _float*				m_ViewMatrix ;

        _float				m_Near ;
		_float				m_Far;
		_float*             m_ProjectionMtx;


    private:
    	virtual void UpdateViewMtx();
    	virtual void UpdateProjectionMtx()=0;
	};

	class PerspCamera : virtual public Camera{
    public:
        PerspCamera(_float FOV, _float Aspect, _float Near=1.0f, _float Far=1000.0f);
        inline void setParameters(_float FOV, _float Aspect, _float Near, _float Far);
        _float getAspectValue();
        _float getFOVValue();

    protected:
        void UpdateProjectionMtx();

    protected:

		_float 				m_FOV ;//Specifies the field of view angle, in degrees for OGL, in the y direction.
        _float				m_Aspect;	//Specifies the aspect ratio that determines the field of view in the x direction.
                   						//The aspect ratio is the ratio of x (width) to y (height).

	};

	class OrthoCamera : virtual public Camera{
    public:
        OrthoCamera(_float Left, _float Right, _float Bottom, _float Top, _float Near, _float Far);
        inline void setParameters(_float Left, _float Right, _float Bottom, _float Top, _float Near, _float Far);
        _float getLeftValue();
        _float getRightValue();
        _float getBottomValue();
        _float getTopValue();

    protected:
        void UpdateProjectionMtx();

    protected:

		_float 				m_Left ;
        _float				m_Right;
        _float              m_Bottom;
        _float              m_Top;


	};

};

#endif
