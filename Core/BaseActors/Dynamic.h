#ifndef _TTB_DYNAMIC_TYPE_H_
#define _TTB_DYNAMIC_TYPE_H_

#include ".//BaseActor.h"

// Dynamic Actor type ID
#define DYNAMIC              0x00000002

namespace TTB{
///Dynamic Type
	class Dynamic: public virtual BaseActor {
	protected:
		Dynamic();
		Dynamic(Vertex3d Pos);
		Dynamic(Vertex3d Pos, Vertex3d Dir, Vertex3d Up);
	public:
		virtual ~Dynamic();
		virtual void Update(_float dt)=0;
	};
};
#endif
