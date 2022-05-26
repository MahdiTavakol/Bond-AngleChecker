/*
* A C++ code to check the initial values for bond and angles from the LAMMPS
* input data file.
*/


#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

struct Atom
{
	int id;
	int type;
	float x, y, z;
};

struct Bond
{
	int id, type, atom1, atom2;
};

struct Angle
{
	int id, type, atom1, atom2, atom3;
};

float distance(const Atom& atom1, const Atom& atom2)
{
	float x1 = atom1.x;
	float y1 = atom1.y;
	float z1 = atom1.z;
    float x2 = atom2.x;
	float y2 = atom2.y;
	float z2 = atom2.z;
	float d = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
	return d;
}

float angle(const Atom& atom1, const Atom& atom2, const Atom& atom3)
{
	float x1 = atom1.x;
	float y1 = atom1.y;
	float z1 = atom1.z;
    float x2 = atom2.x;
	float y2 = atom2.y;
	float z2 = atom2.z;
    float x3 = atom3.x;
	float y3 = atom3.y;
	float z3 = atom3.z;
	float a = distance(atom1,atom2);
	float b = distance(atom2,atom3);
	float dot = (x2-x1)*(x3-x2)+(y2-y1)*(y3-y2)+(z2-z1)*(z3-z2);
	float ang = 180*acos(dot/(a*b))/3.14;
	return ang;
}


int main(int argc, char** argv)
{
	int  atomNum  = 0;
	int  bondNum  = 0;
	int  angleNum = 0;
	vector<Atom> atoms;
	vector<Bond> bonds;
	vector<Angle> angles;
	vector<float> bondLengths;
	vector<float> angleLengths;
	stringstream iss;
	string fileName= argv[1];
	fileName += ".dat";
	ifstream file(fileName);
	ofstream bondFile("Bonds.log");
	ofstream angleFile("Angles.log");

	if(!file)
		cout << "Unable to open!!" << endl;

	string line;

	int lineNumber = 0;
	while(getline(file,line))
	{
		lineNumber++;
		iss.clear();
		iss.str("");
		if(lineNumber == 3)
		{
			string dum;
			iss << line;
			iss >> atomNum;
			iss >> dum;
		}
		else if (lineNumber == 4)
		{
			string dum;
			iss << line;
			iss >> bondNum;
			iss >> dum;
		}
		else if (lineNumber == 5)
		{
			string dum;
			iss << line;
			iss >> angleNum;
			iss >> dum;
		}
		else if(line.length()>=5)
		{
			string head = line.substr(0,5);
			if(!strcmp(head.c_str(),"Atoms"))
				break;
		}
	}


    getline(file,line);

	for (int i = 0; i < atomNum; i++)
	{
		int dumInt, type, id;
		float dumFloat, x, y, z;
		Atom myAtom;
		iss.clear();
		iss.str("");
		getline(file,line);
		iss << line;
		iss >> id;
		iss >> dumInt;
		iss >> type;
		iss >> dumFloat;
		iss >> x;
		iss >> y;
		iss >> z;
		myAtom.id = id;
		myAtom.type = type;
		myAtom.x = x;
		myAtom.y = y;
		myAtom.z = z;
		atoms.push_back(myAtom);
	}

	getline(file,line);
	getline(file,line);
	getline(file,line);

	for (int i = 0; i < bondNum; i++)
	{
		int type;
		int id, atom1, atom2;
		float bLength;
		Bond myBond;
		iss.clear();
		iss.str("");
		getline(file,line);

		iss << line;
		iss >> id;
		iss >> type;
		iss >> atom1;
		iss >> atom2;
		myBond.id = id;
		myBond.type = type;
		myBond.atom1 = atom1;
		myBond.atom2 = atom2;
		bonds.push_back(myBond);
		bLength = distance(atoms[atom1-1],atoms[atom2-1]);
		if (type == 2)
		bondFile << "ids (" << atoms[atom1-1].id << "," << atoms[atom2-1].id << ") types (" << atoms[atom1-1].type << "," << atoms[atom2-1].type <<") = " << bLength << endl;
	}

	getline(file,line);
	getline(file,line);
	getline(file,line);

	for (int i = 0; i < angleNum; i++)
	{
		int type;
		int id, atom1, atom2, atom3;
		Angle myAngle;
		float angleLength;
		iss.clear();
		iss.str("");
		getline(file,line);
		iss << line;
		iss >> id;
		iss >> type;
		iss >> atom1;
		iss >> atom2;
		iss >> atom3;
		myAngle.id = id;
		myAngle.type = type;
		myAngle.atom1 = atom1;
		myAngle.atom2 = atom2;
		myAngle.atom3 = atom3;
		angleLength = angle(atoms[atom1-1],atoms[atom2-1],atoms[atom3-1]);
		if (type == 2 || type == 3 )
		angleFile<< "ids (" << atoms[atom1-1].id << "," << atoms[atom2-1].id << "," << atoms[atom3-1].id <<") types ("
		<< atoms[atom1-1].type << "," << atoms[atom2-1].type <<  "," << atoms[atom3-1].type <<") = " << angleLength << endl;
		angles.push_back(myAngle);
	}

	return 0;
}
