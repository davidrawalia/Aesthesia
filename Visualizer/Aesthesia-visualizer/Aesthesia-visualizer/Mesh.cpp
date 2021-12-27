#include "pch.h"
#include "Mesh.h"

Mesh::Mesh() {
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | 
								COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr)) {
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr)) {
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr)) {
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr)) {
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr)) {
						wFilePath = pszFilePath;
						if (!wFilePath.empty()) {
							int size_needed = WideCharToMultiByte(CP_UTF8, 0, 
																  &wFilePath[0], 
																  (int)wFilePath.size(), 
																  NULL, 0, NULL, NULL);
							filePath = std::string(size_needed, 0);
							WideCharToMultiByte(CP_UTF8, 0, &wFilePath[0], 
												(int)wFilePath.size(), &filePath[0], 
												size_needed, NULL, NULL);
							fileDir = filePath.substr(0, filePath.rfind("\\")) + "\\";
						}
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}

	scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_MaxQuality);
	// If the import failed, report it
	if (!scene) {
		std::cout << "file not found";
	}

	// Extract vertex coordinates
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<glm::vec3> meshVetices;
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshVetices.push_back(glm::vec3(
				scene->mMeshes[i]->mVertices[j].x,
				scene->mMeshes[i]->mVertices[j].y,
				scene->mMeshes[i]->mVertices[j].z));
		}
		meshesVertices.push_back(meshVetices);
	}

	// Extract indices
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<GLuint> meshIndices;
		for (int j = 0; j < scene->mMeshes[i]->mNumFaces; j++) {
			meshIndices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
			meshIndices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
			meshIndices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
		}
		meshesIndices.push_back(meshIndices);
	}

	// Extract normals
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<glm::vec3> meshNormals;
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshNormals.push_back(glm::vec3(
				scene->mMeshes[i]->mNormals[j].x,
				scene->mMeshes[i]->mNormals[j].y,
				scene->mMeshes[i]->mNormals[j].z));
		}
		meshesNormals.push_back(meshNormals);
	}

	// Extract texture coordinates
	for (int i = 0; i < scene->mNumMeshes; i++) {
		std::vector<glm::vec2> meshTexCoords;
		for (int j = 0; j < scene->mMeshes[i]->mNumVertices; j++) {
			meshTexCoords.push_back(glm::vec2(
				scene->mMeshes[i]->mTextureCoords[0][j].x,
				scene->mMeshes[i]->mTextureCoords[0][j].y)
			);
		}
		meshesTexCoords.push_back(meshTexCoords);
	}

	// Extract material indices per mesh
	for (int i = 0; i < scene->mNumMeshes; i++) {
		materialIndices.push_back(scene->mMeshes[i]->mMaterialIndex);
		scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_COLOR_DIFFUSE, 
											       materialColor);
		modelColor.push_back(glm::vec3(materialColor.r, materialColor.g, 
								       materialColor.b));
		scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_COLOR_AMBIENT, 
												   materialReflectionCoefficient);
		reflectionCoefficients.push_back(materialReflectionCoefficient.r);
		scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_REFRACTI, 
												   materialReflectionExponent);
		reflectionExponents.push_back(materialReflectionExponent);
		scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_COLOR_SPECULAR, 
											       materialSpecular);
		speculars.push_back(materialSpecular.r);
	}

	// Mesh VBO and VAO binding
	for (int i = 0; i < scene->mNumMeshes; i++) {
		VBO.push_back(*new GLuint);
		VAO.push_back(*new GLuint);
		normVBO.push_back(*new GLuint);
		texVBO.push_back(*new GLuint);
		EBO.push_back(*new GLuint);

		glGenVertexArrays(1, &VAO[i]);
		glBindVertexArray(VAO[i]);

		glGenBuffers(1, &VBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, 
					 scene->mMeshes[i]->mNumVertices * sizeof(glm::vec3),
                     &meshesVertices[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &normVBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, normVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, 
					 scene->mMeshes[i]->mNumVertices * sizeof(glm::vec3), 
					 &meshesNormals[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &texVBO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, texVBO[i]);
		glBufferData(GL_ARRAY_BUFFER, 
					 scene->mMeshes[i]->mNumVertices * sizeof(glm::vec2), 
					 &meshesTexCoords[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &EBO[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
					 scene->mMeshes[i]->mNumFaces * 3 * sizeof(GLuint),
					 &meshesIndices[i][0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(3, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

const aiScene* Mesh::getScene() {
	return scene;
}

std::vector<GLuint> Mesh::getVAO() {
	return VAO;
}

std::vector<GLuint> Mesh::getVBO() {
	return VBO;
}

std::vector<GLuint> Mesh::getNormVBO() {
	return normVBO;
}

std::vector<GLuint> Mesh::getTexVBO() {
	return texVBO;
}

std::vector<GLuint> Mesh::getEBO() {
	return EBO;
}

std::vector<glm::vec3> Mesh::getModelColor() {
	return modelColor;
}

std::string Mesh::getFileDir() {
	return fileDir;
}

float Mesh::getReflectionCoefficient(int i) {
	return reflectionCoefficients[i];
}

float Mesh::getReflectionExponent(int i) {
	return reflectionExponents[i];
}

float Mesh::getSpecular(int i) {
	return speculars[i];
}