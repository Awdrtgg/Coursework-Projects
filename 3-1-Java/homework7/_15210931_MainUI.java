
package homework7;

import java.util.ArrayList;
import java.awt.*;
import javax.swing.*;

import homework3.FontClass;

public class _15210931_肖蕴明_7_MainUI extends JFrame {
	private _15210931_肖蕴明_7_Controller controller;
	private static final int IW = 960;
    private static final int IH = 900;
    private static final int FW = 1800;
    private static final int FH = 1200;

	public _15210931_肖蕴明_7_MainUI(_15210931_肖蕴明_7_Controller _controller) {
		controller = _controller;
		FontClass.loadIndyFont(); // the font settings
		initUI();
		setSize(FW, FH);
	}

	private void initUI() {
		// the book menu
        JMenuItem mAddBook = new JMenuItem("添加图书");
        mAddBook.addActionListener((e) -> {
			_15210931_肖蕴明_7_AddBookUI addBookUI = new _15210931_肖蕴明_7_AddBookUI(controller);
			addBookUI.setVisible(true);
		});

        JMenuItem mBuy = new JMenuItem("购买图书");
        mBuy.addActionListener((e) -> {
			_15210931_肖蕴明_7_BuyUI buyUI = new _15210931_肖蕴明_7_BuyUI(controller);
			buyUI.setVisible(true);
		});
        
        JMenu mBookOp = new JMenu("图书操作");
        mBookOp.add(mAddBook);
        mBookOp.add(mBuy);
        
        // the strategy menu
        JMenuItem mStrategies = new JMenuItem("设置策略");
        mStrategies.addActionListener((e) -> {
			_15210931_肖蕴明_7_StrategiesUI strategiesUI = new _15210931_肖蕴明_7_StrategiesUI(controller);
			strategiesUI.setVisible(true);
		});

        JMenu mStra = new JMenu("策略操作");
        mStra.add(mStrategies);
        
        JMenuBar mMain = new JMenuBar();
        mMain.add(mBookOp);
        mMain.add(mStra);
        this.setJMenuBar(mMain);

		setTitle("电子书籍购物中心");
		setLocationRelativeTo(null);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}
	
	private static void seedBook(_15210931_肖蕴明_7_Controller controller)
	throws Exception
	{
		ArrayList<_15210931_肖蕴明_7_BookSpecification> books = new ArrayList<_15210931_肖蕴明_7_BookSpecification>();
		books.add(new _15210931_肖蕴明_7_BookSpecification("978-7-302-2", "UML与模式应用", 18, 2));
		books.add(new _15210931_肖蕴明_7_BookSpecification("978-7-312-3", "Java与模式", 34, 1));
		books.add(new _15210931_肖蕴明_7_BookSpecification("968-6-302-1", "HeadFirst设计模式", 58, 1));
		books.add(new _15210931_肖蕴明_7_BookSpecification("958-1-302-2", "爱丽丝历险记", 30, 3));
		books.add(new _15210931_肖蕴明_7_BookSpecification("900-7-392-2", "煲汤大全", 20, 4));

		for (_15210931_肖蕴明_7_BookSpecification book : books)
			controller.addBook(book);
	}

	private static void seedStrategy(_15210931_肖蕴明_7_Controller controller) 
	throws Exception
	{
		_15210931_肖蕴明_7_IPricingStrategy d1 = new _15210931_肖蕴明_7_FlatRateStrategy("Discount001", "绝对值优惠 1", 1);
		_15210931_肖蕴明_7_IPricingStrategy d2 = new _15210931_肖蕴明_7_PercentageStrategy("Discount002", "百分比折扣 1", 7);
		_15210931_肖蕴明_7_IPricingStrategy d3 = new _15210931_肖蕴明_7_PercentageStrategy("Discount003", "百分比折扣 2", 3);
		_15210931_肖蕴明_7_CompositeStrategy d4 = new _15210931_肖蕴明_7_CompositeStrategy("Discount004", "顾客最优策略1");
		d4.add(d1);
		d4.add(d3);

		controller.setStrategy(1, d3);
		controller.setStrategy(2, d1);
		controller.setStrategy(3, d2);
		controller.setStrategy(4, d4);
	}

	private static void seedSale(_15210931_肖蕴明_7_Controller controller) 
	throws Exception
	{
		controller.buyBook("978-7-302-2", 2);
		controller.buyBook("978-7-312-3", 2);
		controller.buyBook("968-6-302-1", 1);
		controller.buyBook("958-1-302-2", 3);
		controller.buyBook("900-7-392-2", 3);
	}

	public static void main(String[] args) {
		// Shared Object
		_15210931_肖蕴明_7_Controller controller = new _15210931_肖蕴明_7_Controller();

		try {
			seedBook(controller);
			seedStrategy(controller);
			seedSale(controller);
		}
		catch(Exception ee) {
			System.out.println(ee.getMessage());
		}

		// Start
		EventQueue.invokeLater(() -> {
			_15210931_肖蕴明_7_MainUI gui = new _15210931_肖蕴明_7_MainUI(controller);
			gui.setVisible(true);
		});
	}
}