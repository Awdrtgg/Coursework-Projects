package homework3;

public class HourlyEmployee extends Employee {
	double wage;
	double hours;
	
	HourlyEmployee(String fn, String ln, String ssn, double w, double h) {
		super(fn, ln, ssn);
		this.wage = w;
		this.hours = h;
	}
	
	public double earning() {
		return (this.wage * this.hours);
	}
	
	public double getWage() {
        return wage;
    }

    public double getHours() {
        return hours;
    }

    public void setWage(double wage) {
        this.wage = wage;
    }

    public void setHours(double hours) {
        this.hours = hours;
    }

    public String toString(){
    	return  "薪水类型：时薪\n" +
    			"名：" + getFirstName() + '\n' +
                "姓：" + getLastName() + '\n' +
                "社保卡号：" + getSocialSecurityNumber() + '\n' +
                "每月薪水：" + String.valueOf(earning()) + '\n';
    }
}
