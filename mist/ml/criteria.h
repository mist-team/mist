#pragma once

namespace mist
{
	struct criteria
	{
		enum criteria_type
		{
			epsilon = 1,
			iteration
		};

		criteria( int type, double eps, int max_itr )
		{
			this->type = type;
			this->eps = eps;
			this->max_itr = max_itr;
		}

		int type;
		double eps;
		int max_itr;
	};
}
