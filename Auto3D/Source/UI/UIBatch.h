#pragma once
#include "../AutoConfig.h"
#include "../Renderer/GeometryNode.h"

namespace Auto3D
{

struct AUTO_API UIBatch
{
	/// Calculate sort _key for state sorting.
	void CalculateSortKey()
	{
		_sortKey = ((((unsigned long long) _geometry) & 0xffff) << 48) |
			((((unsigned long long)_type) & 0xffff) << 32);
	}

	/// Geometry.
	Geometry* _geometry;
	/// Geometry type.
	GeometryType _type;

	union
	{
		/// Sort _key for state sorting.
		unsigned long long _sortKey;
		/// Distance for sorting.
		float _distance;
		/// Instanced mode instance count.
		size_t _instanceCount;
	};
};

/// Per-pass batch queue structure.
struct AUTO_API UIBatchQueue
{
	/// Clear structures.
	void Clear();
	/// Sort batches and build instances.
	void Sort();

	/// Build instances from adjacent batches with same state.
	static void BuildInstances(Vector<UIBatch>& batches);

	/// Batches, which may be instanced or non-instanced.
	Vector<UIBatch> _batches;
	/// Base pass index.
	unsigned char _baseIndex;
	/// Additive pass index (if needed.)
	unsigned char _additiveIndex;
};

}