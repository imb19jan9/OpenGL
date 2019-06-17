#include "CheckerBoard.h"

#include "VBOLayout.h"

CheckerBoard::CheckerBoard(int nx_, int ny_)
	: vaoB(0),
	vboB(0),
	iboB(0),
	vaoW(0),
	vboW(0),
	iboW(0),
	prog(0),
	initialized(0),
	nx(nx_),
	ny(ny_)
{
	Create();
}

CheckerBoard::~CheckerBoard()
{
	delete vaoB;
	delete vboB;
	delete iboB;

	delete vaoW;
	delete vboW;
	delete iboW;

	delete prog;
}

void CheckerBoard::Init()
{
	initialized = true;

	vaoB = new VAO;
	vboB = new VBO(black.GetVertices().data(), black.GetVertices().size() * sizeof(float));
	iboB = new IBO(black.GetIndices().data(), black.GetIndices().size());

	vaoW = new VAO;
	vboW = new VBO(white.GetVertices().data(), white.GetVertices().size() * sizeof(float));
	iboW = new IBO(white.GetIndices().data(), white.GetIndices().size());

	VBOLayout layout;
	layout.Push<float>(3);
	vaoB->AddBuffer(*vboB, layout);
	vaoW->AddBuffer(*vboW, layout);

	prog = new ShaderProgram("res/shaders/BasicColor.vertex", 
		"res/shaders/BasicColor.fragment");
}

void CheckerBoard::Draw(QMatrix4x4 view_, QMatrix4x4 proj_)
{
	if (!initialized)
		return;

	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	QMatrix4x4 mvp = proj_ * view_;
	prog->Bind();
	prog->SetUniformMat4f("u_MVP", mvp.data());

	prog->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f);
	vaoB->Bind();
	iboB->Bind();
	f->glDrawElements(GL_TRIANGLES, iboB->GetCount(), GL_UNSIGNED_INT, 0);

	prog->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	vaoW->Bind();
	iboW->Bind();
	f->glDrawElements(GL_TRIANGLES, iboW->GetCount(), GL_UNSIGNED_INT, 0);
}

void CheckerBoard::Create()
{
	float squareSize = 10.0f;
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			TriMesh::Point p0((i + 0)*squareSize, (j + 0)*squareSize, 0);
			TriMesh::Point p1((i + 1)*squareSize, (j + 0)*squareSize, 0);
			TriMesh::Point p2((i + 1)*squareSize, (j + 1)*squareSize, 0);
			TriMesh::Point p3((i + 0)*squareSize, (j + 1)*squareSize, 0);

			std::vector< TriMesh::VertexHandle> vhs;
			if ((i + j) % 2 != 0) {
				vhs.push_back(black.add_vertex(p0));
				vhs.push_back(black.add_vertex(p1));
				vhs.push_back(black.add_vertex(p2));
				vhs.push_back(black.add_vertex(p3));

				black.add_face(vhs[0], vhs[1], vhs[2]);
				black.add_face(vhs[2], vhs[3], vhs[0]);
			}
			else {
				vhs.push_back(white.add_vertex(p0));
				vhs.push_back(white.add_vertex(p1));
				vhs.push_back(white.add_vertex(p2));
				vhs.push_back(white.add_vertex(p3));
				
				white.add_face(vhs[0], vhs[1], vhs[2]);
				white.add_face(vhs[2], vhs[3], vhs[0]);
			}
		}
	}

	black.Update();
	white.Update();
}
