#pragma once

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

struct HCCLTraits : public OpenMesh::DefaultTraits
{
	VertexAttributes(OpenMesh::Attributes::Normal);
	FaceAttributes(OpenMesh::Attributes::Normal);
	EdgeAttributes(0);
};

class TriMesh : public OpenMesh::TriMesh_ArrayKernelT<HCCLTraits>
{
private:
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

public:
	void Clear();
	bool Read(std::string filePath_);
	bool Write(std::string filePath_);
	void Update();

	inline const std::vector<float>& GetVertices() { return vertices; }
	inline const std::vector<unsigned int>& GetIndices() { return indices; }
};