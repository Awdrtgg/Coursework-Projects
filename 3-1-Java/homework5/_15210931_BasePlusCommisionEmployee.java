package homework5;

public class _15210931_肖蕴明_5_BasePlusCommisionEmployee extends _15210931_肖蕴明_5_CommisionEmployee {
	double baseSalary;
	
	_15210931_肖蕴明_5_BasePlusCommisionEmployee(String fn, String ln, String ssn, double g, double c, double b) {
		super(fn, ln, ssn, g, c);
		this.baseSalary = b;
	}
	
	public double earning() {
		return (this.grossSales * this.commissionRate + this.baseSalary);
	}
	
    public double getBaseSalary() {
        return baseSalary;
    }
    
    public void setBaseSalary(double baseSalary) {
        this.baseSalary = baseSalary;
    }
    
    public String toString(){
    	return  "薪水类型：基础+提成\n" +
    			"名：" + getFirstName() + '\n' +
                "姓：" + getLastName() + '\n' +
                "社保卡号：" + getSocialSecurityNumber() + '\n' +
                "每月薪水：" + String.valueOf(earning()) + '\n';
    }
}
