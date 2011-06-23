#include "UIDisplayRoleChoose.h"
#include "RenderSystem.h"
#include "UIDialog.h"
#include "Graphics.h"
#include "Intersect.h"
#include "RPGSkyUIGraph.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CUIDisplayRoleChoose::CUIDisplayRoleChoose()
{
	// # The perspective of the camera parameters generated (生成摄像机的视角参数)
	// # ----
	m_vEye			= Vec3D(0.0f,0.0f,-1.0f);
	m_vLookAt		= Vec3D(0.0f,0.0f,0.0f);
	// ----
	// # Light Direction
	// ----
	m_lightDir		= DirectionalLight(Vec4D(0.4f, 0.4f, 0.4f, 0.4f), Vec4D(1.0f, 1.0f, 1.0f, 1.0f), Vec4D(0.6f, 0.6f, 0.6f, 0.6f), Vec3D(1.0f, -0.4f, 1.0f));
	// ----
	m_pModelObject	= NULL;
	// ----
	for(size_t i = 0 ; i < MAX_VISUAL_ROLE ; i++)
	{
		m_pRole[i]	= NULL;
	}
	// ----
	m_nSelectIndex = -1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CUIDisplayRoleChoose::~CUIDisplayRoleChoose()
{
	delete m_pModelObject;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::LoadModel(const char * szFilename)
{
	S_DEL(m_pModelObject);
	// ----
	m_pModelObject = new CModelObject();
	m_pModelObject->load(szFilename);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnFrameMove(double fTime, float fElapsedTime)
{
	// # Update the view matrix (更新视矩阵)
	// # Render the update		(渲染更新)
	// ----
	if(m_pModelObject != NULL)
	{
		m_pModelObject->OnFrameMove(fElapsedTime);
		m_pModelObject->updateEmitters(Matrix::UNIT, fElapsedTime);
	}
	// ----
	for(size_t i = 0 ; i < MAX_VISUAL_ROLE ; i++)
	{
		if(m_pRole[i] != NULL)
		{
			m_pRole[i]->OnFrameMove(fElapsedTime);
			m_pRole[i]->updateEmitters(Matrix::UNIT,fElapsedTime);
		}
	}
	// ----
	CUIDisplay::OnFrameMove(fTime, fElapsedTime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnFrameRender(const Matrix& mTransform, double fTime, float fElapsedTime )
{
	CUIDisplay::OnFrameRender(mTransform, fTime, fElapsedTime);
	// ----
	if((IsVisible() == true) && (isStyleVisible() == true))
	{
		Vec3D vUp(0, 1, 0);
		// ----
		CRenderSystem & R		= GetRenderSystem();
		CRect<int> rcViewport	= getViewport();
		CShader * pShader		= NULL;
		// ----
		float fAspect			= (rcViewport.getWidth() / (float)rcViewport.getHeight());
		// ----
		// # Clear back screen to zero (背屏清0)
		// ----
		R.ClearBuffer(true, false, 0x0);
		// ----
		// Projection matrix (投影矩阵)
		// ----
		m_mProj.MatrixPerspectiveFovLH(PI /4 , fAspect, 0.01f, 128.0f);
		// ----
		R.setProjectionMatrix(m_mProj);
		// ----
		m_mView.MatrixLookAtLH(m_vEye, m_vLookAt, vUp);
		// ----
		pShader = R.GetShaderMgr().getSharedShader();
		// ----
		pShader->setFloat("g_fTime", fTime);
		pShader->setMatrix("g_mViewProj", m_mProj * m_mView);
		pShader->setMatrix("g_mView", m_mView);
		pShader->setVec3D("g_vLightDir", m_vEye);
		pShader->setVec3D("g_vEyePot", m_vEye);
		// ----
		// # Light
		// ----
		GetRenderSystem().SetDirectionalLight(0, m_lightDir);
		// ----
		R.setWorldMatrix(Matrix::UNIT);
		R.setViewMatrix(m_mView);
		// ----
		// # Set window size (设置视窗大小)
		// ----
		R.setViewport(rcViewport);
		// ----
		R.SetSamplerFilter(0, TEXF_LINEAR, TEXF_LINEAR, TEXF_LINEAR);
		R.SetSamplerFilter(1, TEXF_LINEAR, TEXF_LINEAR, TEXF_LINEAR);
		// ----
		if(m_pModelObject != NULL)
		{
			// # Rendering model (渲染模型)
			// ----
			m_pModelObject->render();
		}
		// ----
		if((m_nSelectIndex >= 0) && (m_nSelectIndex < MAX_VISUAL_ROLE))
		{
			if(m_pRole[m_nSelectIndex] != NULL)
			{
				// # Show selected role.
				// ----
				m_pRole[m_nSelectIndex]->renderFocus(0xFFFFFF40);
			}
		}
		// ----
		// # Render the role
		// ----
		for(size_t i = 0 ; i < MAX_VISUAL_ROLE ; i++)
		{
			if(m_pRole[i] != NULL)
			{
				m_pRole[i]->render(MATERIAL_RENDER_NORMAL);
			}
		}
		// ----
		// # Render the name
		// ----
		RPGSkyUIGraph::getInstance().initDrawText();
		// ----
		for(size_t i = 0 ; i < MAX_VISUAL_ROLE ; i++)
		{
			if(m_pRole[i] != NULL)
			{
				m_pRole[i]->drawName();
			}
		}
		// ----
		// # Render the bump
		// ----
		if(m_pModelObject != NULL)
		{
			CTexture * pSceneTexture = m_SceneEffect.getSceneTexture();
			// ----
			pShader->setTexture("g_texScene", pSceneTexture);
			// ----
			R.SetBlendFunc(false);
			// ----
			m_pModelObject->render(MATERIAL_RENDER_BUMP, MATERIAL_RENDER_BUMP);
		}
		// ----
		R.SetupRenderState();
		// ----
		R.setViewport(GetParentDialog()->GetBoundingBox());
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnMouseMove(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnMouseWheel(POINT point,short wheelDelta)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnLButtonDown(POINT point)
{
	Vec3D vRayPos;
	Vec3D vRayDir;
	// ----
	float fMin = 0.00;
	float fMax = 0.00;
	// ----
	GetPickRay(vRayPos, vRayDir, point.x, point.y, m_mView, m_mProj, m_rcBoundingBox.getRECT());
	// ----
	for(int i = 0 ; i < MAX_VISUAL_ROLE ; i++)
	{
		if(m_pRole[i] != NULL)
		{
			if(m_pRole[i]->intersect(vRayPos , vRayDir, fMin, fMax) == true)
			{
				m_nSelectIndex = i;
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnLButtonUp(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnRButtonDown(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnRButtonUp(POINT point)
{
	// ----
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CUIDisplayRoleChoose::OnSize(const CRect<int>& rc)
{
	CUIDisplay::OnSize(rc);
	// ----
	m_SceneEffect.Reset(GetBoundingBox());
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CModelObject * CUIDisplayRoleChoose::getModelObject()
{
	return m_pModelObject;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CModelData * CUIDisplayRoleChoose::getModelData()const
{
	CModelData * pReturn = NULL;
	// ----
	if(m_pModelObject != NULL)
	{
		pReturn = m_pModelObject->getModelData();
	}
	// ----
	return pReturn;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------