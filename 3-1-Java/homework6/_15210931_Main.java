
package homework6;

import java.util.*;

public class _15210931_肖蕴明_6_Main {
	public static void main(String[] args) {
		try {
			_15210931_肖蕴明_6_ProductSpecification spec1 = new _15210931_肖蕴明_6_ProductSpecification("978-7-302-2", "UML与模式应用", 18, 2);
			_15210931_肖蕴明_6_ProductSpecification spec2 = new _15210931_肖蕴明_6_ProductSpecification("978-7-312-3", "Java与模式", 34, 1);
			_15210931_肖蕴明_6_ProductSpecification spec3 = new _15210931_肖蕴明_6_ProductSpecification("968-6-302-1", "HeadFirst设计模式", 58, 1);
			_15210931_肖蕴明_6_ProductSpecification spec4 = new _15210931_肖蕴明_6_ProductSpecification("958-1-302-2", "爱丽丝历险记", 30, 3);
			_15210931_肖蕴明_6_ProductSpecification spec5 = new _15210931_肖蕴明_6_ProductSpecification("900-7-392-2", "煲汤大全", 20, 4);
		
			_15210931_肖蕴明_6_SaleLineItem saleLineItem1 = new _15210931_肖蕴明_6_SaleLineItem(2, spec1);
			_15210931_肖蕴明_6_SaleLineItem saleLineItem2 = new _15210931_肖蕴明_6_SaleLineItem(2, spec2);
			_15210931_肖蕴明_6_SaleLineItem saleLineItem3 = new _15210931_肖蕴明_6_SaleLineItem(1, spec3);
			_15210931_肖蕴明_6_SaleLineItem saleLineItem4 = new _15210931_肖蕴明_6_SaleLineItem(3, spec4);
			_15210931_肖蕴明_6_SaleLineItem saleLineItem5 = new _15210931_肖蕴明_6_SaleLineItem(1, spec5);
			
			_15210931_肖蕴明_6_Sale sale = new _15210931_肖蕴明_6_Sale();
			sale.addItem(saleLineItem1);
			sale.addItem(saleLineItem2);
			sale.addItem(saleLineItem3);
			sale.addItem(saleLineItem4);
			sale.addItem(saleLineItem5);
			
			System.out.println("本次购物一共花销" + sale.getTotal() + "元");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
