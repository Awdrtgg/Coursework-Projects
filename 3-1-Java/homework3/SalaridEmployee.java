package homework3;

public class SalaridEmployee extends Employee {
	double weeklySalary;
	
	SalaridEmployee(String fn, String ln, String ssn, double s) {
		super(fn, ln, ssn);
		this.weeklySalary = s;
	}
	
	public double earning() {
		return (4.0 * this.weeklySalary);
	}
	
	public double getWeeklySalary() {
        return weeklySalary;
    }

    public void setWeeklySalary(double weeklySalary) {
        this.weeklySalary = weeklySalary;
    }

    public String toString(){
        return  "薪水类型：周薪\n" +
                "名：" + getFirstName() + '\n' +
                "姓：" + getLastName() + '\n' +
                "社保卡号：" + getSocialSecurityNumber() + '\n' +
                "每月薪水：" + String.valueOf(earning()) + '\n';
    }
}
