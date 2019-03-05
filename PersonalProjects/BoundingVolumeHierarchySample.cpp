
//recursive algorithm
void Tree::TopDownRec(std::vector<glm::vec3> NodeVerts, node** currnode, int level)
{
	//create a new node to enter into the tree
	*currnode = new node;
	(*currnode)->level = level;
	GenerateBoundingVolumes(NodeVerts, *currnode);

	//split the verts to the left or to the right based on the longest axis
	std::vector<glm::vec3> leftVerts;
	std::vector<glm::vec3> rightVerts;
	for(auto & vertex : NodeVerts)
	{
		//if the longest axis is the X axis then insert left if the vert X is less than the object center X
		if((*currnode)->LAxis == BoundingVolume::XAXIS)
		{
			if(vertex.x < (*currnode)->center.x)
			{
				leftVerts.push_back(vertex);
			}
			else
			{
				rightVerts.push_back(vertex);
			}
		}
		//if the longest axis is the Y axis then insert left if the vert Y is less than the object center Y
		else if ((*currnode)->LAxis == BoundingVolume::YAXIS)
		{
			if (vertex.y < (*currnode)->center.y)
			{
				leftVerts.push_back(vertex);
			}
			else
			{
				rightVerts.push_back(vertex);
			}
		}
		//if the longest axis is the Z axis then insert left if the vert Z is less than the object center Z
		else if ((*currnode)->LAxis == BoundingVolume::ZAXIS)
		{
			if (vertex.z < (*currnode)->center.z)
			{
				leftVerts.push_back(vertex);
			}
			else
			{
				rightVerts.push_back(vertex);
			}
		}
	}
	//acting base cases
	//do not insert if there are less than 500 vertices in the child
	if(rightVerts.size() > 500)
	{
		TopDownRec(rightVerts, &((*currnode)->rightChild), level + 1);
	}
	if(leftVerts.size() > 500)
	{
		TopDownRec(leftVerts, &((*currnode)->leftChild), level + 1);
	}
}

void Tree::BuildTopDown()
{
	//make sure the tree is empty
	if(m_head)
	{
		DeleteNodes(m_head);
	}
	//m_head = new(node);
	//call recursive function
	TopDownRec(m_verts, &m_head, 1);
}

//iterative algorithm
void Tree::BuildBottomUp()
{
	if (m_head)
	{
		DeleteNodes(m_head);
	}
	std::vector<node*> objects;
	for (auto & object : m_bottomupverts)
	{
		node * newnode = new node;
		GenerateBoundingVolumes(object, newnode);
		newnode->verts = object;
		objects.push_back(newnode);
	}
	//condense those volumes
	CondenseNodes(objects);
	m_head = (objects[0]);
	//AssignLevels(m_head, 1);
}

void Tree::CondenseNodes(std::vector<node*> & nodes)
{
	//set the level of each node
	for(auto & currnode : nodes)
	{
		currnode->level = 1;
	}

	//while there are more than one node in the list
	while(nodes.size() > 1)
	{
		//for the current node, find the next closest node
		float shortestdist = INFINITY;
		int shortestindex = 0;
		for(int j = 1; j < nodes.size(); ++j)
		{
			float dist = glm::distance(nodes[0]->center, nodes[j]->center);
			if(dist < shortestdist)
			{
				shortestdist = dist;
				shortestindex = j;
			}
		}
		//create a new node to hold the current node and its closest neighbor
		node * newnode = new node;
		//determine the level of the new node
		if(nodes[0]->level > nodes[shortestindex]->level)
		{
			newnode->level = nodes[0]->level + 1;
		}
		else
		{
			newnode->level = nodes[shortestindex]->level + 1;
		}
		//loop through the current nodes vertices and put them in the new node
		for(auto & verts : nodes[0]->verts)
		{
			newnode->verts.push_back(verts);
		}
		//loop through the neighbor nodes vertices and put them in the new node
		for(auto & verts : nodes[shortestindex]->verts)
		{
			newnode->verts.push_back(verts);
		}
		//generate the bounding volumes of the new node
		GenerateBoundingVolumes(newnode->verts, newnode);
		//set the relations of the new node
		newnode->leftChild = nodes[0];
		newnode->rightChild = nodes[shortestindex];
		//place the new node in the vector and remove its children
		nodes.push_back(newnode);
		nodes.erase(nodes.begin() + shortestindex);
		nodes.erase(nodes.begin());
	}
}
