#include "Mesh.hpp"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Mesh::init() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(3, VBOs);
	glGenBuffers(1, &EBO);

	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// UV
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(Vector2), uv.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	// Triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(uint), triangles.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::Mesh(const std::vector<Vector3>& vertices, const std::vector<Vector2>& uv, const std::vector<Vector3>& normals, const std::vector<uint>& triangles) : vertices(vertices), uv(uv), normals(normals), triangles(triangles) {
	init();
}

/*Mesh::Mesh(const std::string& path) {
	std::ifstream file(path);
	
	std::vector<Vector3> objVertices;
	std::vector<Vector2> objUv;
	std::vector<Vector3> objNormals;
	std::vector<uint> objTriangles;

	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string type;
		ss >> type;

		if (type == "v") {
			Vector3 v;
			ss >> v.x >> v.y >> v.z;
			objVertices.emplace_back(v);
		} else if (type == "vt") {
			Vector2 vt;
			ss >> vt.x >> vt.y;
			objUv.emplace_back(vt);
		} else if (type == "vn") {
			Vector3 vn;
			ss >> vn.x >> vn.y >> vn.z;
			objNormals.emplace_back(vn);
		} else if (type == "f") {
			std::string facePart;
			while (ss >> facePart) {
				uint firstSlash = facePart.find('/');
				uint secondSlash = facePart.find('/', firstSlash + 1);


			}
		}
	}
}*/

Mesh::Mesh(const char* path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Assimp critical error! While importing " << path << " Assimp said: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Process each mesh in the scene
	for (uint i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

		// Extract vertices
		for (uint j = 0; j < mesh->mNumVertices; j++) {
			vertices.push_back({ mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z });
		}

		// Extract UV coordinates (assuming a single UV channel)
		if (mesh->HasTextureCoords(0)) {
			for (uint j = 0; j < mesh->mNumVertices; j++) {
				uv.push_back({ mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y });
			}
		}
		else {
			// Handle meshes without UVs (e.g., initialize with default values)
			for (uint j = 0; j < mesh->mNumVertices; j++) {
				uv.push_back({ 0.0f, 0.0f });
			}
		}

		// Extract normals
		if (mesh->HasNormals()) {
			for (uint j = 0; j < mesh->mNumVertices; j++) {
				normals.push_back({ mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z });
			}
		}
		else {
			// Handle meshes without normals (e.g., generate or initialize with default values)
			for (uint j = 0; j < mesh->mNumVertices; j++) {
				normals.push_back({ 0.0f, 0.0f, 0.0f });
			}
		}

		// Extract triangles (faces)
		for (uint j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			for (uint k = 0; k < face.mNumIndices; k++) {
				triangles.push_back(face.mIndices[k]);
			}
		}
	}

	init();
}

Mesh::~Mesh() {
	glDeleteBuffers(3, VBOs);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
	VBOs[0] = 0;
	VBOs[1] = 0;
	VBOs[2] = 0;
	EBO = 0;
	VAO = 0;
}