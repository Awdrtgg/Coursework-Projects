package homework5;

import javax.swing.UIManager; 
import javax.swing.*;
import java.awt.*;
import java.util.Vector;

public class _15210931_Ð¤ÔÌÃ÷_5_Main extends JFrame{
    public _15210931_Ð¤ÔÌÃ÷_5_Factory factory = new _15210931_Ð¤ÔÌÃ÷_5_Factory();
    private static final int IW = 960;
    private static final int IH = 900;
    private static final int FW = 1800;
    private static final int FH = 1200;
    private static final int CW = 600;
    private static final int CH = 300;
    
    public static void main(String[] args) {
        EventQueue.invokeLater(() -> {
            _15210931_Ð¤ÔÌÃ÷_5_Main gui = new _15210931_Ð¤ÔÌÃ÷_5_Main();
            gui.setVisible(true);
        });
    }

    public _15210931_Ð¤ÔÌÃ÷_5_Main() {
        _15210931_Ð¤ÔÌÃ÷_5_FontClass.loadIndyFont(); 
        initUI();
    }

    private void initUI() {
        initMenu();
        this.add(new JLabel ("Pretend to be some background...", JLabel.CENTER));

        setTitle("Employee Management System");
        setSize(FW, FH);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE); 
    }

    private void initMenu()
    {
    	// the first menu
        JMenuItem mBPCEmp = new JMenuItem("BasePlusCommisionEmployee");
        mBPCEmp.addActionListener((e) -> {
            BasePlusCommisionteDialog dialog = new BasePlusCommisionteDialog(this);
            dialog.setVisible(true);
        });

        JMenuItem mCEmp = new JMenuItem("CommisionEmployee");
        mCEmp.addActionListener((e) -> {
            CommisionEmpDialog dialog = new CommisionEmpDialog(this);
            dialog.setVisible(true);
        });
        
        JMenu mEmpInfoInput = new JMenu("EmployeeInfoInput");
        mEmpInfoInput.add(mBPCEmp);
        mEmpInfoInput.add(mCEmp);

        // the second menu
        JMenuItem mAverSearch = new JMenuItem("AverageEarningSearch");
        mAverSearch.addActionListener((e) -> {
            AverEarningDialog dialog = new AverEarningDialog();
            dialog.setVisible(true);
        });

        JMenu mSearch = new JMenu("Search");
        mSearch.add(mAverSearch);

        JMenuBar mMain = new JMenuBar();
        mMain.add(mEmpInfoInput);
        mMain.add(mSearch);
        this.setJMenuBar(mMain);
    }

    class BasePlusCommisionteDialog extends JDialog {
        public BasePlusCommisionteDialog(JFrame f) {
            super(f);
            initUI();
        }

        private void initUI() {
            JPanel panel = new JPanel();
            panel.setLayout(new GridLayout(7, 2));

            panel.add(new JLabel("Input the first name: "));
            JTextField firstNameField = new JTextField();
            panel.add(firstNameField);

            panel.add(new JLabel("Input the last name: "));
            JTextField lastNameField = new JTextField();
            panel.add(lastNameField);

            panel.add(new JLabel("Input the social security number"));
            JTextField secNumField = new JTextField();
            panel.add(secNumField);

            panel.add(new JLabel("The base salary: "));
            JTextField baseSalaryField = new JTextField();
            panel.add(baseSalaryField);

            panel.add(new JLabel("The gross sale amount: "));
            JTextField salesField = new JTextField();
            panel.add(salesField);

            panel.add(new JLabel("The commission rate: "));
            JTextField rateField = new JTextField();
            panel.add(rateField);

            JButton bSubmit= new JButton("Submit");
            bSubmit.addActionListener((event) -> {
                try {
                    // Make sure security number is not empty
                    if (secNumField.getText().equals(""))
                        throw new IllegalArgumentException();
                    
                    _15210931_Ð¤ÔÌÃ÷_5_BasePlusCommisionEmployee temp = new _15210931_Ð¤ÔÌÃ÷_5_BasePlusCommisionEmployee(
                    		firstNameField.getText(),
                            lastNameField.getText(),
                            secNumField.getText(),
                            Double.parseDouble(salesField.getText()),
                            Double.parseDouble(rateField.getText()),
                            Double.parseDouble(baseSalaryField.getText())
                    );
                    _15210931_Ð¤ÔÌÃ÷_5_Employee result = factory.addEmployee(temp);
                    SuccessDlg log = new SuccessDlg();
                    log.setVisible(true);
                    this.dispose();
                }
                catch (Exception e) {
                	ErrorDlg log = new ErrorDlg();
                    log.setVisible(true);
                }
            });
            panel.add(bSubmit);

            JButton bCancel = new JButton("Cancel");
            bCancel.addActionListener ((event) -> {
                this.dispose();
            });
            panel.add(bCancel);

            // Add panel
            this.add(panel);
            this.pack();

            setTitle("Create a new employee");
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
            setLocationRelativeTo(getParent());
            setSize(IW, IH);
        }
    }

    private class CommisionEmpDialog extends JDialog {
        public CommisionEmpDialog(JFrame f) {
            super(f);
            initUI();
        }

        private void initUI() {
        	JPanel panel = new JPanel();
            panel.setLayout(new GridLayout(6, 2));

            panel.add(new JLabel("Input the first name: "));
            JTextField firstNameField = new JTextField();
            panel.add(firstNameField);

            panel.add(new JLabel("Input the last name: "));
            JTextField lastNameField = new JTextField();
            panel.add(lastNameField);

            panel.add(new JLabel("Input the social security number"));
            JTextField secNumField = new JTextField();
            panel.add(secNumField);

            panel.add(new JLabel("The gross sale amount: "));
            JTextField salesField = new JTextField();
            panel.add(salesField);

            panel.add(new JLabel("The commission rate: "));
            JTextField rateField = new JTextField();
            panel.add(rateField);

            JButton bSubmit= new JButton("Submit");
            bSubmit.addActionListener((event) -> {
                try {
                    // Make sure security number is not empty
                    if (secNumField.getText().equals(""))
                        throw new IllegalArgumentException();
                    
                    _15210931_Ð¤ÔÌÃ÷_5_CommisionEmployee temp = new _15210931_Ð¤ÔÌÃ÷_5_CommisionEmployee(
                            firstNameField.getText(),
                            lastNameField.getText(),
                            secNumField.getText(),
                            Double.parseDouble(salesField.getText()),
                            Double.parseDouble(rateField.getText())
                    );
                    _15210931_Ð¤ÔÌÃ÷_5_Employee result = factory.addEmployee(temp);
                    SuccessDlg log = new SuccessDlg();
                    log.setVisible(true);
                    this.dispose();
                }
                catch (Exception e) {
                    ErrorDlg log = new ErrorDlg();
                    log.setVisible(true);
                }
            });
            panel.add(bSubmit);

            JButton bCancel = new JButton("Cancel");
            bCancel.addActionListener ((event) -> {
                this.dispose();
            });
            panel.add(bCancel);

            // Add panel
            this.add(panel);
            this.pack();

            setTitle("Create Commision Employee");
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
            setLocationRelativeTo(getParent());
            setSize(IW, IH);
        }
    }

    class ErrorDlg extends JDialog {
        public ErrorDlg() {
            super();
            initUI();
        }

        private void initUI() {
            // show error info
            this.add(new JLabel("Invalid input.", JLabel.CENTER));
            this.pack();

            setTitle("Error");
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
            setLocationRelativeTo(getParent());
            setSize(CW, CH);
            this.setModal(true);
        }
    }
    
    class SuccessDlg extends JDialog {
        public SuccessDlg() {
            super();
            initUI();
        }

        private void initUI() {
            // show info
            this.add(new JLabel("successfully operated.", JLabel.CENTER));
            this.pack();

            setTitle ("Hint");
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
            setLocationRelativeTo(getParent());
            setSize(CW, CH);
            this.setModal(true);
        }
    }

    class AverEarningDialog extends JDialog {
        public AverEarningDialog() {
            super();
            initUI();
        }

        private void initUI() {
            this.add(new JLabel("The average Earning: " + factory.averageSalary(), JLabel.CENTER));
            this.pack();

            setTitle ("Average Earning");
            setDefaultCloseOperation(DISPOSE_ON_CLOSE);
            setLocationRelativeTo(getParent());
            setSize(CW, CH);
        }
    }
}