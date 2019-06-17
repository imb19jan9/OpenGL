#include "TriMesh.h"

void TriMesh::Clear()
{
	vertices.clear();
	indices.clear();
	clear();
}

bool TriMesh::Read(std::string filePath_)
{
	// Add vertex normals as default property (ref. previous tutorial)
	request_vertex_normals();
	// Add face normals as default property
	request_face_normals();

	OpenMesh::IO::Options ropt;
	if (!OpenMesh::IO::read_mesh(*this, filePath_, ropt)) {
		std::cerr << "File Open Error: Error loading mesh from file " 
			<< filePath_ << std::endl;
		return false;
	}

	// If the file did not provide vertex normals, then calculate them
	if (!ropt.check(OpenMesh::IO::Options::VertexNormal) &&
		has_face_normals() && has_vertex_normals()) {
		// let the mesh update the normals
		update_normals();
	}

	
	for (TriMesh::VertexIter vit = vertices_begin();
		vit != vertices_end();
		vit++) {
		TriMesh::Point p = point(*vit);
		
		vertices.push_back(p[0]);
		vertices.push_back(p[1]);
		vertices.push_back(p[2]);
	}
	
	for (TriMesh::FaceIter fit = faces_begin();
		fit != faces_end();
		fit++) {
		for (TriMesh::FVIter fvit = fv_begin(*fit);
			fvit != fv_end(*fit);
			fvit++) {
			int idx = fvit->idx();
			indices.push_back(fvit->idx());
		}
	}

	return true;
}

bool TriMesh::Write(std::string filePath_)
{
	OpenMesh::IO::Options wopt;
	if (!OpenMesh::IO::write_mesh(*this, filePath_, wopt)) {
		std::cerr << "Cannot write mesh to file" 
			<< filePath_ << std::endl;
		return false;
	}

	return true;
}

void TriMesh::Update()
{
	vertices.clear();
	indices.clear();

	for (TriMesh::VertexIter vit = vertices_begin();
		vit != vertices_end();
		vit++) {
		TriMesh::Point p = point(*vit);

		vertices.push_back(p[0]);
		vertices.push_back(p[1]);
		vertices.push_back(p[2]);
	}

	for (TriMesh::FaceIter fit = faces_begin();
		fit != faces_end();
		fit++) {
		for (TriMesh::FVIter fvit = fv_begin(*fit);
			fvit != fv_end(*fit);
			fvit++) {
			indices.push_back(fvit->idx());
		}
	}
}
