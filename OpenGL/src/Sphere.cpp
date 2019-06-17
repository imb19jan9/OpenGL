#include "Sphere.h"

#include "VBOLayout.h"

Sphere::Sphere(int sectorCount_, int stackCount_)
	: vao(0),
	vbo(0),
	ibo(0),
	prog(0),
	radius(1),
	sectorCount(sectorCount_),
	stackCount(stackCount_)
{
	BuildMesh();
}

Sphere::Sphere(float radius_, int sectorCount_, int stackCount_)
	: vao(0),
	vbo(0),
	ibo(0),
	prog(0),
	radius(radius_),
	sectorCount(sectorCount_),
	stackCount(stackCount_)
{
	BuildMesh();
}

Sphere::~Sphere()
{
	delete vao;
	delete vbo;
	delete ibo;
	
	delete prog;
}

void Sphere::Init()
{
	vao = new VAO;

	vbo = new VBO(mesh.GetVertices().data(), sizeof(float)*mesh.GetVertices().size());
	ibo = new IBO(mesh.GetIndices().data(), mesh.GetIndices().size());

	VBOLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	prog = new ShaderProgram("res/shaders/Phong.vertex",
		"res/shaders/Phong.fragment");
}

void Sphere::Draw(QMatrix4x4 view_, QMatrix4x4 proj_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	QMatrix4x4 model, model1, model2;
	model1.scale(radius);
	model2.translate(pos);
	model = model2 * model1;
	QMatrix4x4 mv = view_* model;
	prog->Bind();
	prog->SetUniformMat4f("u_ModelView", mv.data());
	prog->SetUniformMat4f("u_Proj", proj_.data());

	prog->SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);
	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}

void Sphere::BuildMesh()
{
	mesh.Clear();

	float stackStep = M_PI / stackCount;
	float sectorStep = 2 * M_PI / sectorCount;

	for (int i = 0; i <= stackCount; i++) {
		float stackAngle = M_PI / 2 - stackStep * i;
		float xy = cos(stackAngle);
		float z = sin(stackAngle);

		// add (sectorCount+1) vertices per stack
		for (int j = 0; j <= sectorCount; j++) {
			float sectorAngle = sectorStep * j;
			float x = xy * cos(sectorAngle);
			float y = xy * sin(sectorAngle);

			mesh.add_vertex(TriMesh::Point(x, y, z));
		}
	}

	for (int i = 0; i < stackCount; i++) {
		int k1 = i * (sectorCount + 1);
		int k2 = k1 + sectorCount + 1;

		for (int j = 0; j < sectorCount; j++, k1++, k2++) {
			std::vector<TriMesh::VertexHandle> vhs;
			if (i != 0) {
				vhs.push_back(mesh.vertex_handle(k1));
				vhs.push_back(mesh.vertex_handle(k2));
				vhs.push_back(mesh.vertex_handle(k1 + 1));
				mesh.add_face(vhs);
				vhs.clear();
			}

			if (i != (stackCount - 1)) {
				vhs.push_back(mesh.vertex_handle(k1 + 1));
				vhs.push_back(mesh.vertex_handle(k2));
				vhs.push_back(mesh.vertex_handle(k2 + 1));
				mesh.add_face(vhs);
			}
		}
	}

	mesh.Update();
}
