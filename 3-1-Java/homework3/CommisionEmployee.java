package homework3;

public class CommisionEmployee extends Employee {
	double grossSales;
	double commissionRate;
	
	CommisionEmployee(String fn, String ln, String ssn, double g, double c) {
		super(fn, ln, ssn);
		this.grossSales = g;
		this.commissionRate = c;
	}
	
	public double earning() {
		return (this.grossSales * this.commissionRate);
	}
	
	public double getGrossSales() {
        return grossSales;
    }

    public double getCommissionRate() {
        return commissionRate;
    }

    public void setGrossSales(double grossSales) {
        this.grossSales = grossSales;
    }

    public void setCommissionRate(double commissionRate) {
        this.commissionRate = commissionRate;
    }

    public String toString(){
    	return  "薪水类型：提成\n" +
    			"名：" + getFirstName() + '\n' +
                "姓：" + getLastName() + '\n' +
                "社保卡号：" + getSocialSecurityNumber() + '\n' +
                "每月薪水：" + String.valueOf(earning()) + '\n';
    }
}
