

class AggregatePrimitive {
public:
	vector<Primitive*> list;

	AggregatePrimitive(){};
	AggregatePrimitive(vector<Primitive*>);
	bool intersect(Ray&, float*, Intersection*);
	bool intersectP(Ray&);
	void getBRDF(LocalGeo&, BRDF*);
};