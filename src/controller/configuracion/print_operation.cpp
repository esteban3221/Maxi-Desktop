#include "controller/configuracion/print_operation.hpp"

PrintFormOperation::PrintFormOperation()
{
}

PrintFormOperation::~PrintFormOperation()
{
}

Glib::RefPtr<PrintFormOperation> PrintFormOperation::create()
{
  return Glib::make_refptr_for_instance<PrintFormOperation>(new PrintFormOperation());
}

void PrintFormOperation::on_begin_print(const Glib::RefPtr<Gtk::PrintContext>& print_context)
{
  //Create and set up a Pango layout for PrintData based on the passed
  //PrintContext: We then use this to calculate the number of pages needed, and
  //the lines that are on each page.
  m_refLayout = print_context->create_pango_layout();

  Pango::FontDescription font_desc("mono 9");
  m_refLayout->set_font_description(font_desc);

  const double width = print_context->get_width();
  const double height = print_context->get_height();

  m_refLayout->set_width(static_cast<int>(width * Pango::SCALE));


  m_refLayout->set_markup(this->markup);
  Pango::TabArray tabs(2);
  tabs.set_tab(0, Pango::TabAlign::LEFT, 0);
  tabs.set_tab(1, Pango::TabAlign::RIGHT, 150); 

  m_refLayout->set_tabs(tabs);

  this->markup.clear(); 

  //Set the number of pages to print by determining the line numbers
  //where page breaks occur:
  const int line_count = m_refLayout->get_line_count();

  Glib::RefPtr<Pango::LayoutLine> layout_line;
  double page_height = 0;

  for (int line = 0; line < line_count; ++line)
  {
    Pango::Rectangle ink_rect, logical_rect;

    layout_line = m_refLayout->get_line(line);
    layout_line->get_extents(ink_rect, logical_rect);

    const double line_height = logical_rect.get_height() / 1024.0;

    if (page_height + line_height > height)
    {
      m_PageBreaks.push_back(line);
      page_height = 0;
    }

    page_height += line_height;
  }

  set_n_pages(m_PageBreaks.size() + 1);
}

void PrintFormOperation::on_draw_page(const Glib::RefPtr<Gtk::PrintContext>& print_context, int page_nr)
{
  //Decide which lines we need to print in order to print the specified page:
  int start_page_line = 0;
  int end_page_line = 0;

  if(page_nr == 0)
  {
    start_page_line = 0;
  }
  else
  {
    start_page_line = m_PageBreaks[page_nr - 1];
  }

  if(page_nr < static_cast<int>(m_PageBreaks.size()))
  {
    end_page_line = m_PageBreaks[page_nr];
  }
  else
  {
    end_page_line = m_refLayout->get_line_count();
  }

  Cairo::RefPtr<Cairo::Context> cairo_ctx = print_context->get_cairo_context();

  cairo_ctx->set_source_rgb(0, 0, 0);
  auto iter = m_refLayout->get_iter();

  double start_pos = 0;
  int line_index = 0;

  do
  {
    if(line_index >= start_page_line)
    {
      auto layout_line = iter.get_line();
      auto logical_rect = iter.get_line_logical_extents();
      int baseline = iter.get_baseline();

      if (line_index == start_page_line)
      {
        start_pos = logical_rect.get_y() / 1024.0;
      }

      cairo_ctx->move_to(logical_rect.get_x() / 1024.0,
        baseline / 1024.0 - start_pos);

      layout_line->show_in_cairo_context(cairo_ctx);
    }

    line_index++;
  }
  while(line_index < end_page_line && iter.next_line());
}