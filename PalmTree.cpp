#include "PalmTree.h"




PalmTree::PalmTree(int numLeaves, int numLeaflets, float radius) {

	// Parameters for the palm tree
	this->numLeaflets = numLeaflets;
	this->numLeaves = numLeaves;
	this->radius = radius;




    trunkControlPoints = {
    glm::vec3(0.0f,0.0f,0.5f),
    glm::vec3(1.0f,2.0f,0.7f),
    glm::vec3(3.5f,3.5f,0.9f),
    glm::vec3(3.5f,5.0f,1.1f)
    };

    lowerTrunkControlPoints = {
        glm::vec3(-2.0f,-3.0f,0.5f),// P0
        trunkControlPoints[0],  // P1 (connect here)
        glm::vec3(-1.5f,-0.5f,0.7f),// T0
        trunkControlPoints[2]   // T1 (match upper start tangent)
    };

    endPoint = trunkControlPoints[1];

    // Palm Leaves
    palmCrown.Position = endPoint;
    palmCrown.Scale = { 1.0f, 1.0f, 1.0f };

	plotTrunkAndLeaves();
	build(trunkControlPoints, lowerTrunkControlPoints);

}

void PalmTree::plotTrunkAndLeaves() {
	// This function is used to plot the trunk and leaves of the palm tree
    leaves.reserve(numLeaves);

    for (int i = 0; i < numLeaves; i++)
    {
        float angle = (float)i / (float)numLeaves * 2.0f * M_PI;

        float x = cos(angle) * radius;
        float z = sin(angle) * radius;

        std::vector<glm::vec3> leafcontrolPoints = { {
            glm::vec3(0.0f, 0.0f, 0.0f),// origin
            glm::vec3(x * 0.5, 0.7f, z * 0.7f),
            glm::vec3(x * 0.5f, 1.3f, z * 0.9f),
            glm::vec3(x * 1.0f, 0.7f, z * 1.1f) //endpoint
         } };


        leaves.emplace_back();
        leaves.back().build(leafcontrolPoints);

        //Leaflets Memory reserve
        leaflets.push_back({});
        leaflets.back().reserve(numLeaflets);
        leafletTransforms.push_back({});
        leafletTransforms.back().reserve(numLeaflets);



        for (int j = 0; j < numLeaflets; j++)
        {
            float t = (float)j / (numLeaflets - 1);

            cout << "t: " << t << endl;

            glm::vec3 leafletStart = EvaluateBezier(
                leafcontrolPoints[0],
                leafcontrolPoints[1],
                leafcontrolPoints[2],
                leafcontrolPoints[3],
                t
            );

            // find the tangent
            glm::vec3 tangent = glm::normalize(BezierTangent(
                leafcontrolPoints[0],
                leafcontrolPoints[1],
                leafcontrolPoints[2],
                leafcontrolPoints[3],
                t
            ));

            glm::vec3 up = glm::vec3(0, 1, 0);

            glm::vec3 binormal = glm::normalize(glm::cross(up, tangent));
            glm::vec3 normal = glm::normalize(glm::cross(tangent, binormal));


            Transform transform;
            transform.Position = palmCrown.Position + leafletStart;
            transform.Rotation = binormal;
            transform.Scale = { 1.0f, 1.0f, 1.0f };
            leafletTransforms[i].emplace_back(transform);

            std::vector<glm::vec3> leafletControlPoints = { {
                glm::vec3(-0.5, -0.7f, -0.7f),
                glm::vec3(0.0f, 0.0f, 0.0f),// origin (using B-Splin)
                glm::vec3(0.5f, 1.7f,  0.9f),
                glm::vec3(1.5f, 0.3f, 1.1f) //endpoint
         } };

            leaflets[i].emplace_back();
            leaflets[i].back().build(leafletControlPoints);


        }

		// Trunk Curve Mesh Initialization

        upperCurveTransform.Position = { 0.0f, 0.0f, 0.0f };
        upperCurveTransform.Scale = { 1.0f, 1.0f, 1.0f };

        upperCurveMesh.init();

        lowercurveTransform.Position = { 0.0f, 0.0f, 0.0f };
        lowercurveTransform.Scale = { 1.0f, 1.0f, 1.0f };


        lowercurveMesh.init();
    };


}


void PalmTree::updatePalmTree() {
    // This function is used to update the palm tree's properties 
    leaves.clear();
    leaflets.clear();
    leafletTransforms.clear();

	plotTrunkAndLeaves();
	build(trunkControlPoints, lowerTrunkControlPoints);

}



void PalmTree::build(std::vector<glm::vec3> trunkControlPoints,
    std::vector<glm::vec3> lowerTrunkControlPoints
) {
	lowercurveMesh.build(lowerTrunkControlPoints);
    upperCurveMesh.build(trunkControlPoints);
};


void PalmTree::release() {
    lowercurveMesh.release();
    upperCurveMesh.release();
    for (auto& leaf : leaves) {
        leaf.release();
    }
    for (auto& leafletRow : leaflets) {
        for (auto& leaflet : leafletRow) {
            leaflet.release();
        }
    }
}

void PalmTree::update(Renderer renderer, Camera camera) {

    // Render the palm tree trunk curve (Hermite curves)
    renderer.DrawCurve(
        upperCurveMesh,
        upperCurveTransform,
        camera,
        trunkColor,
        BasisMatrices::hermite
    );

    renderer.DrawCurve(
        lowercurveMesh,
        lowercurveTransform,
        camera,
        trunkColor,
        BasisMatrices::hermite
    );

    // Iterate through leaves and leaflets to render them
    for (int i = 0; i < leaves.size(); i++)
    {
        renderer.DrawCurve(leaves[i], palmCrown, camera, leafColor, BasisMatrices::bezier);

        for (int j = 0; j < leaflets[i].size(); j++)
        {
            CurveMesh& leaflet = leaflets[i][j];

            Transform transform = leafletTransforms[i][j];

            renderer.DrawCurve(
                leaflet,
                transform,
                camera,
                glm::vec4(0.1f, 0.7f, 0.1f, 1.0f),
                BasisMatrices::bspline
            );

            Transform flipped = transform;
            flipped.Scale.x *= -1;

            renderer.DrawCurve(
                leaflet,
                flipped,
                camera,
                glm::vec4(0.1f, 0.7f, 0.1f, 1.0f),
                BasisMatrices::bspline
            );
        }
    }
}


PalmTree::~PalmTree() {
    // Cleanup resources if needed
	lowercurveMesh.release();
    upperCurveMesh.release();

    for (auto& leaf : leaves) {
        leaf.release();
	}

    for (auto& leafletRow : leaflets) {
        for (auto& leaflet : leafletRow) {
            leaflet.release();
        }
	}
}





