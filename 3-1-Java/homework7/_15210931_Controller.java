
package homework7;

import java.util.ArrayList;
import java.util.HashMap;

public class _15210931_肖蕴明_7_Controller
{
	private _15210931_肖蕴明_7_BookCatalog bookCatalog;
	private _15210931_肖蕴明_7_StrategyCatalog strategyCatalog;
	private _15210931_肖蕴明_7_Sale sale;

	public _15210931_肖蕴明_7_Controller() {
		bookCatalog = new _15210931_肖蕴明_7_BookCatalog();
		strategyCatalog = new _15210931_肖蕴明_7_StrategyCatalog();
		sale = new _15210931_肖蕴明_7_Sale();

		_15210931_肖蕴明_7_PricingStrategyFactory.getInstance().setCatalog(strategyCatalog);
	}

	// AddBookUI
	public void addBook(_15210931_肖蕴明_7_BookSpecification newBook) 
	throws Exception {
		bookCatalog.addBook(newBook);
	}

	public ArrayList<_15210931_肖蕴明_7_BookSpecification> getBooks() {
		return bookCatalog.getBooks();
	}

	public String[] getCategoryStrings() {
		return _15210931_肖蕴明_7_BookCatalog.categoryStrings;
	}

	// StrategiesUI
	public void setStrategy(int category, _15210931_肖蕴明_7_IPricingStrategy strategy) {
		strategyCatalog.setStrategy(category, strategy);
	}

	public void deleteStrategy(int category) {
		strategyCatalog.removeStrategy(category);
	}

	public HashMap<Integer, _15210931_肖蕴明_7_IPricingStrategy> getStrategies() {
		return strategyCatalog.getStrategies();
	}

	public String[] getStrategyTypeStrings() {
		return _15210931_肖蕴明_7_StrategyCatalog.typeStrings;
	}

	// BuyUI
	public void buyBook(String isbn, int copies) 
	throws Exception
	{
		if (copies < 0)
			throw new Exception("份数不能小于0");

		_15210931_肖蕴明_7_BookSpecification book = bookCatalog.getBook (isbn);
		sale.addItem(new _15210931_肖蕴明_7_SaleLineItem(copies, book));
		sale.notifyObservers();
	}

	public _15210931_肖蕴明_7_Sale getSale() {
		return sale;
	}
}