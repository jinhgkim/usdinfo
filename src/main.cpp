#include <iostream>
#include <fstream>
#include <string>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/primRange.h>
#include <pxr/usd/usdgeom/gprim.h>

using namespace std;
using namespace pxr;

int main(int argc, char *argv[])
{
    // Check the number of parameters
    if (argc < 2 || argc > 3)
    {
        // Tell the user how to run the program
        cerr << "Usage: " << argv[0] << " PATH PRIM" << endl;
        return 1;
    }

    string filePath = argv[1];
    string targetPrim = argv[2];
    // string newColor = argv[3];

    cout << "Opening " << filePath << endl;
    UsdStageRefPtr stage = UsdStage::Open(filePath);
    if (!stage)
    {
        cerr << "ERROR: Failed to open " << filePath << endl;
        return 1;
    }

    // Create and open a text file
    ofstream outputFile("usdinfo_output.txt");

    int count = 0;
    for (auto prim : stage->Traverse())
    {
        if (prim.GetName() == targetPrim)
        {
            cout << "Applying new color to " << targetPrim << endl;
            auto newColor = GfVec3f(0.9f, 0.75f, 0.55f);
            auto children = prim.GetChildren();
            for (auto child : children)
            {
                UsdGeomGprim gprim(child);
                gprim.GetDisplayColorAttr().Set(VtArray<GfVec3f>({newColor}));
                outputFile << child.GetPath() << " (" << child.GetName() << ")" << endl;
                count++;
            }
        }
    }
    stage->GetRootLayer()->Save();
    outputFile << "Total number of " << targetPrim << ": " << count << endl;

    // Close the file
    outputFile.close();
    return 0;
}