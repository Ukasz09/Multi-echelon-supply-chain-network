#pragma once
class MscGeneratorRanges
{
public:
	static const double FROM_SUPPLIERS_TO_FACTORIES_COST_MAX;
	static const double FROM_FACTORIES_TO_STORE_COST_MAX;
	static const double FROM_STORE_TO_SHOPS_COST_MAX;
	static const double FROM_SUPPLIERS_TO_FACTORIES_COST_MIN;
	static const double FROM_FACTORIES_TO_STORE_COST_MIN;
	static const double FROM_STORE_TO_SHOPS_COST_MIN;
	static const double ONE_TIME_COST_FOR_SUPPLIER_MAX;
	static const double ONE_TIME_COST_FOR_FACTORIES_MAX;
	static const double ONE_TIME_COST_FOR_SUPPLIER_MIN;
	static const double ONE_TIME_COST_FOR_STORE_MIN;
	static const double ONE_TIME_COST_FOR_FACTORIES_MIN;
	static const double SUPPLIERS_MANUFACTURING_CAPACITY_MAX;
	static const double FACTORIES_MANUFACTURING_CAPACITY_MAX;
	static const double STORE_MANUFACTURING_CAPACITY_MAX;
	static const double SHOPS_MANUFACTURING_CAPACITY_MAX;
	static const double SUPPLIERS_MANUFACTURING_CAPACITY_MIN;
	static const double FACTORIES_MANUFACTURING_CAPACITY_MIN;
	static const double STORE_MANUFACTURING_CAPACITY_MIN;
	static const double SHOPS_MANUFACTURING_CAPACITY_MIN;
	static const double PROFIT_FROM_ONE_PIECE_IN_SHOPS_MAX;
	static const double PROFIT_FROM_ONE_PIECE_IN_SHOPS_MIN;
	static const double ONE_TIME_COST_FOR_STORE_MAX;
	
private:
	MscGeneratorRanges()=delete;
	~MscGeneratorRanges()=delete;
};

