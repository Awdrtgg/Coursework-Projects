package homework5;

public class _15210931_Ф����_5_BasePlusCommisionEmployee extends _15210931_Ф����_5_CommisionEmployee {
	double baseSalary;
	
	_15210931_Ф����_5_BasePlusCommisionEmployee(String fn, String ln, String ssn, double g, double c, double b) {
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
    	return  "нˮ���ͣ�����+���\n" +
    			"����" + getFirstName() + '\n' +
                "�գ�" + getLastName() + '\n' +
                "�籣���ţ�" + getSocialSecurityNumber() + '\n' +
                "ÿ��нˮ��" + String.valueOf(earning()) + '\n';
    }
}
