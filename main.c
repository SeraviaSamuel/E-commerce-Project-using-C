#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
COORD coord = {0, 0};

void gotoxy(int x, int y)
{
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void SetColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

typedef struct Product Product;
typedef struct Category Category;

struct Product
{
    char productName[20];
    float productPrice;
    int productquantity;
    int productCategory;
};

struct Category
{
    char categoryName[20];
    int numofproducts;
    Product *products;
};

void Display(Category *categories, int numofcat)
{
   if(numofcat==0)
   {
       printf("\n\n SORRY , But There is No Categories To Display\n");
   }
   else
   {
       for (int j = 0; j < numofcat; j++)
    {
        printf("Category %d Name : %s\n", j + 1, categories[j].categoryName);
        printf("Products in Category %d:\n", j + 1);

        for (int k = 0; k < categories[j].numofproducts; k++)
        {
            printf("Product %d Name :%s , Quantity :%d , Price :%.2f $ \n", k + 1, categories[j].products[k].productName,
                   categories[j].products[k].productquantity, categories[j].products[k].productPrice);
        }
    }
   }
}

void Move(Category *categories, int numofcat)
{
    if(numofcat==0)
    {
        printf("\n\n SORRY , But There is No Categories To Move Products \n");
    }
    else
    {
            char sourcecategory[20];
    printf("Enter Source Category Name of Product you need to Move:");
    scanf("%s", sourcecategory);

    char movedproduct[20];
    printf("Enter Name of Product you need to Move:");
    scanf("%s", movedproduct);

    char destinationcategory[20];
    printf("Enter Destination Category Name of Product you need to Move:");
    scanf("%s", destinationcategory);

    int sourcecategoryindex = -1;
    int destinationcategoryindex = -1;
    int movedproductindex = -1;

    for (int k = 0; k < numofcat; k++)
    {
        if (strcmp(sourcecategory, categories[k].categoryName) == 0)
        {
            sourcecategoryindex = k;
        }

        if (strcmp(destinationcategory, categories[k].categoryName) == 0)
        {
            destinationcategoryindex = k;
        }
    }

    if (sourcecategoryindex == -1 || destinationcategoryindex == -1)
    {
        printf("Source or destination category not found!\n");
        return;
    }

    for (int j = 0; j < numofcat; j++)
    {
        if (strcmp(movedproduct, categories[sourcecategoryindex].products[j].productName) == 0)
        {
            movedproductindex = j;
        }
    }

    if (movedproductindex == -1)
    {
        printf("Product not found in the source category!\n");
        return;
    }

    if (sourcecategoryindex != -1 && destinationcategoryindex != -1 && movedproductindex != -1)
    {
        categories[destinationcategoryindex].products = realloc(categories[destinationcategoryindex].products, (categories[destinationcategoryindex].numofproducts + 1) * sizeof(Product));
        categories[destinationcategoryindex].products[categories[destinationcategoryindex].numofproducts] = categories[sourcecategoryindex].products[movedproductindex];
        categories[destinationcategoryindex].numofproducts++;

        for (int i = movedproductindex; i < categories[sourcecategoryindex].numofproducts - 1; i++)
        {
            categories[sourcecategoryindex].products[i] = categories[sourcecategoryindex].products[i + 1];
        }

        categories[sourcecategoryindex].numofproducts--;
        categories[sourcecategoryindex].products = realloc(categories[sourcecategoryindex].products, categories[sourcecategoryindex].numofproducts * sizeof(Product));

        printf("Product Moved Successfully");
    }

    }
}

void Sell(Category *categories, int numofcat)
{
    if(numofcat==0)
    {
        printf("\n\n SORRY , But There is No Categories To SELL Products\n");
    }
    else
    {
         float totalprice=0;
    while (1)
    {
        char categoryname[20];
        char productname[20];
        printf("Enter Category Name (type 'exit' to exit): ");
        scanf("%s", categoryname);

        if (strcmp(categoryname, "exit") == 0)
        {
            printf("Exit Selling Process\n");
            break;
        }

        int categoryindex = -1;
        int productindex = -1;
        int numofproducts = 0;
        int productquantity = 0;

        for (int j = 0; j < numofcat; j++)
        {
            if (strcmp(categoryname, categories[j].categoryName) == 0)
            {
                categoryindex = j;
            }
        }
        if (categoryindex == -1)
        {
            printf("Category Not Found\n");
            continue;
        }

        printf("\nEnter Number of Products to sell: ");
        scanf("%d", &numofproducts);

        for (int k = 0; k < numofproducts; k++)
        {
            printf("Enter Product Name you Want (type 'exit' to choose a different category): ");
            scanf("%s", productname);

            if (strcmp(productname, "exit") == 0)
            {
                printf("Choosing a different category\n");
                break;
            }

            for (int i = 0; i < categories[categoryindex].numofproducts; i++)
            {
                if (strcmp(productname, categories[categoryindex].products[i].productName) == 0)
                {
                    productindex = i;
                    break;
                }
            }

            if (productindex == -1)
            {
                printf("Product Not Found\n");
                continue;
            }

            printf("Enter Number of Quantity you Need From %s : ", productname);
            scanf("%d", &productquantity);

            if (productquantity > categories[categoryindex].products[productindex].productquantity)
            {
                printf("There is No Enough Quantity\n");
                break;
            }

            categories[categoryindex].products[productindex].productquantity= categories[categoryindex].products[productindex].productquantity-productquantity;
            totalprice=totalprice+(categories[categoryindex].products[productindex].productPrice*productquantity);

            if (categories[categoryindex].products[productindex].productquantity == 0)
            {
                printf("This Product is Deleted Because You Sell All The Quantity\n");
                for (int a = productindex; a < categories[categoryindex].numofproducts - 1; a++)
                {
                    categories[categoryindex].products[a] = categories[categoryindex].products[a + 1];
                }
                categories[categoryindex].numofproducts--;
                categories[categoryindex].products = realloc(categories[categoryindex].products, categories[categoryindex].numofproducts * sizeof(Product));
            }

            printf("Product Sold Successfully\n");
        }
    }
    printf("Total Price for all products: %.2f $ \n ",totalprice);

    }

}

int main()
{
    int selected = 0;
    Category *categories = NULL;
    int newnumofcat = 0;
    int totalnumofcat = 0;
    int numofproducts;
    int option;
    while (1)
    {
        system("cls");
        gotoxy(5,5);

        if (selected == 0)
        {
            SetColor(0x0C);
            printf("-> Add New categories and Products Or Add Products to Existing Category");
            SetColor(0x0F);
        }
        else
        {
            printf("   Add New categories and Products Or Add Products to Existing Category");
        }

        gotoxy(5,6);

        if (selected == 1)
        {
            SetColor(0x0C);
            printf("-> Display categories and Product");
            SetColor(0x0F);
        }
        else
        {
            printf("   Display categories and Product");
        }

        gotoxy(5,7);

        if (selected == 2)
        {
            SetColor(0x0C);
            printf("-> Move Product From Category to Other Category");
            SetColor(0x0F);
        }
        else
        {
            printf("   Move Product From Category to Other Category");
        }

        gotoxy(5,8);

        if (selected == 3)
        {
            SetColor(0x0C);
            printf("-> Sell Products");
            SetColor(0x0F);
        }
        else
        {
            printf("   Sell Products");
        }

        gotoxy(5,9);

        if (selected == 4)
        {
            SetColor(0x0C);
            printf("-> Exit\n");
            SetColor(0x0F);
        }
        else
        {
            printf("   Exit\n");
        }

        char key = _getch();
        switch (key)
        {

        case 72:
            if (selected == 0)
            {
                selected = 4;
            }
            else
            {
                selected--;
            }
            break;

        case 80:
            if (selected == 4)
            {
                selected = 0;
            }
            else
            {
                selected++;
            }
            break;

        case 13:
            if (selected == 0)
            {
                system("cls");
                printf(" If You want to Add New Category Press 1 or If You want to Add Product to Existing Category Press 2 : ");
                scanf("%d", &option);

                if (option == 1)
                {
                    int categoryexist = 0;
                    printf("Enter Num of category:");
                    scanf("%d", &newnumofcat);

                    categories = realloc(categories, (totalnumofcat + newnumofcat) * sizeof(Category));

                    for (int j = totalnumofcat; j < totalnumofcat + newnumofcat;j++)
                    {
                        printf("Enter Category Name:");
                        scanf("%s", categories[j].categoryName);
                        categoryexist = 0;
                        for (int i = 0; i < totalnumofcat + newnumofcat; i++)
                        {
                            if (strcmp(categories[j].categoryName, categories[i].categoryName) == 0 && j != i)
                            {
                                categoryexist = 1;
                                printf("***Category Already Exist With This Name**\n");
                                newnumofcat--;
                                break;
                            }
                        }
                        if (categoryexist == 0)
                        {
                            int numofproducts;
                            printf("Enter Number of Products for %s :", categories[j].categoryName);
                            scanf("%d", &numofproducts);
                            categories[j].products = malloc(numofproducts * sizeof(Product));
                            for (int k = 0; k < numofproducts; k++)
                            {
                                printf("Enter Product %d Name:", k + 1);
                                scanf("%s", categories[j].products[k].productName);
                                printf("Enter Product %d Quantity:", k + 1);
                                scanf("%d", &categories[j].products[k].productquantity);

                                while (categories[j].products[k].productquantity < 0)
                                {
                                    printf("**Invalid Quantity , Please Enter It Again**\n");
                                    printf("Enter Product %d Quantity:", k + 1);
                                    scanf("%d", &categories[j].products[k].productquantity);
                                }

                                printf("Enter Product %d Price:", k + 1);
                                scanf("%f", &categories[j].products[k].productPrice);

                                while (categories[j].products[k].productPrice < 0)
                                {
                                    printf("**Invalid Price , Please Enter It Again**\n");
                                    printf("Enter Product %d Price:", k + 1);
                                    scanf("%f", &categories[j].products[k].productPrice);
                                }

                                categories[j].products[k].productCategory = j;
                            }

                            categories[j].numofproducts = numofproducts;
                            printf("**%s  Category Added Successfully**\n",categories[j].categoryName);
                        }
                    }
                    totalnumofcat = totalnumofcat + newnumofcat;
                    newnumofcat = 0;
                    //printf("Added Successfully");
                }
                else if (option == 2)
                {
                    if (totalnumofcat == 0)
                    {
                        printf("\n\n SORRY , But There is No Existing Category To Add Products Inside It");
                    }
                    else
                    {
                        int categoryindex = -1;
                        char categoryname[20];
                        printf("Enter category Name you need to add Products Inside it:");
                        scanf("%s",categoryname);
                        for(int i=0;i<totalnumofcat;i++)
                        {
                            if(strcmp(categoryname,categories[i].categoryName)==0)
                            {
                                categoryindex=i;
                                break;
                            }
                        }
                        if(categoryindex!=-1)
                        {
                            int numofnewproduct = 0;
                            printf("Enter Number Of Products to Add:");
                            scanf("%d", &numofnewproduct);

                            categories[categoryindex].products = realloc(categories[categoryindex].products, (categories[categoryindex].numofproducts + numofnewproduct) * sizeof(Product));

                            for (int k = categories[categoryindex].numofproducts; k < categories[categoryindex].numofproducts + numofnewproduct; k++)
                            {
                                printf("Enter New Product Name:");
                                scanf("%s", categories[categoryindex].products[k].productName);
                                printf("Enter New Product Price:");
                                scanf("%f", &categories[categoryindex].products[k].productPrice);
                                while(categories[categoryindex].products[k].productPrice<0)
                                {
                                    printf("**Invalid Price , Please Enter It Again**\n");
                                    printf("Enter new product price:");
                                    scanf("%f", &categories[categoryindex].products[k].productPrice);
                                }
                                printf("Enter new product Quantity:");
                                scanf("%d", &categories[categoryindex].products[k].productquantity);
                                while(categories[categoryindex].products[k].productquantity<=0)
                                {
                                    printf("**Invalid Quantity , Please Enter It Again**\n");
                                    printf("Enter new product Quantity:");
                                    scanf("%d", &categories[categoryindex].products[k].productquantity);
                                }
                                categories[categoryindex].products[k].productCategory = categoryindex;
                            }
                            categories[categoryindex].numofproducts = categories[categoryindex].numofproducts + numofnewproduct;
                            printf("Product Added Successfully");
                        }
                        else
                        {
                            printf("Category Not Found \n");
                        }
                    }
                }
                key = getch();
            }

            if (selected == 1)
            {
                system("cls");
                Display(categories, totalnumofcat);
                key = getch();
            }

            if (selected == 2)
            {
                system("cls");
                Move(categories, totalnumofcat);
                key = getch();
            }

            if (selected == 3)
            {
                system("cls");
                Sell(categories, totalnumofcat);
                key = getch();
            }

            if (selected == 4)
            {
                system("cls");
                printf("Exiting\n");
                return 0;
            }

            break;

        case 27:
            system("cls");
            return 0;
            break;
        }
    }

    return 0;
}
