#include "Tetrahedron.h"

using namespace TRACKBALL;

Tetrahedron::Tetrahedron(float scaleSize_)
	: vao(0),
	vbo(0),
	ibo(0),
	prog(0),
	scaleSize(scaleSize_)
{
	Create();
}

Tetrahedron::~Tetrahedron()
{
	delete vao;
	delete vbo;
	delete ibo;

	delete prog;
}

void Tetrahedron::Init()
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	mesh.GetVertices(vertices, true, false);
	mesh.GetIndices(indices);

	vao = new VAO;
	vbo = new VBO(vertices.data(), vertices.size() * sizeof(float));

	VBOLayout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	vao->AddBuffer(*vbo, layout);

	ibo = new IBO(indices.data(), indices.size());

	prog = new ShaderProgram("res/shaders/Phong.vertex",
		"res/shaders/Phong.fragment");
}

void Tetrahedron::Draw(QMatrix4x4 view_, QMatrix4x4 proj_)
{
	QOpenGLFunctions_4_5_Core *f = QOpenGLContext::currentContext()->
		versionFunctions<QOpenGLFunctions_4_5_Core>();

	QMatrix4x4 model, model1, model2;
	model1.scale(scaleSize);
	model2.translate(pos);
	model = model2 * model1;

	QMatrix4x4 mv = view_ * model;
	prog->Bind();
	prog->SetUniformMat4f("u_ModelView", mv.data());
	prog->SetUniformMat4f("u_Proj", proj_.data());

	prog->SetUniform4f("u_Color", color[0], color[1], color[2], color[3]);
	vao->Bind();
	ibo->Bind();
	f->glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, 0);
}

void Tetrahedron::Create()
{
	TriMesh::Point v1(sqrt(8 / 9.0), 0, -1 / 3.0);
	TriMesh::Point v2(-sqrt(2 / 9.0), sqrt(2 / 3.0), -1 / 3.0);
	TriMesh::Point v3(-sqrt(2 / 9.0), -sqrt(2 / 3.0), -1 / 3.0);
	TriMesh::Point v4(0, 0, 1);

	TriMesh::VertexHandle vh1 = mesh.add_vertex(v1);
	TriMesh::VertexHandle vh2 = mesh.add_vertex(v2);
	TriMesh::VertexHandle vh3 = mesh.add_vertex(v3);
	TriMesh::VertexHandle vh4 = mesh.add_vertex(v4);

	mesh.add_face(vh1, vh3, vh2);
	mesh.add_face(vh1, vh2, vh4);
	mesh.add_face(vh2, vh3, vh4);
	mesh.add_face(vh1, vh4, vh3);
}