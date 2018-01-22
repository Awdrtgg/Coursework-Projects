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
    	return  "нˮ���ͣ����\n" +
    			"����" + getFirstName() + '\n' +
                "�գ�" + getLastName() + '\n' +
                "�籣���ţ�" + getSocialSecurityNumber() + '\n' +
                "ÿ��нˮ��" + String.valueOf(earning()) + '\n';
    }
}
