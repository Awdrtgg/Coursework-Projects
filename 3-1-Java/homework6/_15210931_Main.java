
package homework6;

import java.util.*;

public class _15210931_Ф����_6_Main {
	public static void main(String[] args) {
		try {
			_15210931_Ф����_6_ProductSpecification spec1 = new _15210931_Ф����_6_ProductSpecification("978-7-302-2", "UML��ģʽӦ��", 18, 2);
			_15210931_Ф����_6_ProductSpecification spec2 = new _15210931_Ф����_6_ProductSpecification("978-7-312-3", "Java��ģʽ", 34, 1);
			_15210931_Ф����_6_ProductSpecification spec3 = new _15210931_Ф����_6_ProductSpecification("968-6-302-1", "HeadFirst���ģʽ", 58, 1);
			_15210931_Ф����_6_ProductSpecification spec4 = new _15210931_Ф����_6_ProductSpecification("958-1-302-2", "����˿���ռ�", 30, 3);
			_15210931_Ф����_6_ProductSpecification spec5 = new _15210931_Ф����_6_ProductSpecification("900-7-392-2", "������ȫ", 20, 4);
		
			_15210931_Ф����_6_SaleLineItem saleLineItem1 = new _15210931_Ф����_6_SaleLineItem(2, spec1);
			_15210931_Ф����_6_SaleLineItem saleLineItem2 = new _15210931_Ф����_6_SaleLineItem(2, spec2);
			_15210931_Ф����_6_SaleLineItem saleLineItem3 = new _15210931_Ф����_6_SaleLineItem(1, spec3);
			_15210931_Ф����_6_SaleLineItem saleLineItem4 = new _15210931_Ф����_6_SaleLineItem(3, spec4);
			_15210931_Ф����_6_SaleLineItem saleLineItem5 = new _15210931_Ф����_6_SaleLineItem(1, spec5);
			
			_15210931_Ф����_6_Sale sale = new _15210931_Ф����_6_Sale();
			sale.addItem(saleLineItem1);
			sale.addItem(saleLineItem2);
			sale.addItem(saleLineItem3);
			sale.addItem(saleLineItem4);
			sale.addItem(saleLineItem5);
			
			System.out.println("���ι���һ������" + sale.getTotal() + "Ԫ");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
