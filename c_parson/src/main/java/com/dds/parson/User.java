package com.dds.parson;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by dds on 2019/10/22.
 * android_shuai@163.com
 */
public class User {

    private String name;
    private String occu;
    private ArrayList<String> habbits;

    public User() {
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getOccu() {
        return occu;
    }

    public void setOccu(String occu) {
        this.occu = occu;
    }

    public List<String> getHabbits() {
        return habbits;
    }

    public void setHabbits(ArrayList<String> habbits) {
        this.habbits = habbits;
    }


}
