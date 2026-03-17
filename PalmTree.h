#pragma once

#include "curveMesh.h"
#include "Transform.h"
#include "BasisMatrices.h"
#include "BezierEvaluation.h"
#include <corecrt_math_defines.h>
#include <iostream>
#include "Renderer.h"

using namespace std;

class PalmTree {
	public:
		int numLeaves = 7;
		int numLeaflets = 8;
		float radius = 1.5f;



		std::vector<glm::vec3> trunkControlPoints;
		std::vector<glm::vec3> lowerTrunkControlPoints;
		glm::vec3 endPoint;

		Transform palmCrown;

		//Color values
		glm::vec4 leafletColor = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);
		glm::vec4 leafColor = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f);
		glm::vec4 trunkColor = glm::vec4(0.709f, 0.39f, 0.1f, 1.0f);
		
		// Curve Meshes
		CurveMesh upperCurveMesh;
		CurveMesh lowercurveMesh;

		Transform upperCurveTransform;
		Transform lowercurveTransform;

		// Leaves and Leaflets
		std::vector<CurveMesh> leaves;
		std::vector <std::vector<CurveMesh>> leaflets;
		std::vector < std::vector<Transform>> leafletTransforms;

		void build(
			std::vector<glm::vec3> trunkControlPoints,
			std::vector<glm::vec3> lowerTrunkControlPoints
		);

		void update(Renderer renderer, Camera camera);

		PalmTree(int numLeaves, int numLeaflets, float radius);

		void plotTrunkAndLeaves();

		void updatePalmTree();

		void release();

		~PalmTree();



private:

};